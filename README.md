# Application de Gestion Innovante pour Boutique ou Restaurant

## Description

Ce projet consiste à créer une application moderne et innovante de gestion de boutique ou de restaurant en utilisant les technologies Qt (C++ avec Qt Widgets/QML) et MySQL.

Pour une description complète des objectifs et de la feuille de route, consultez le fichier [CAHIER_DE_CHARGE.md](CAHIER_DE_CHARGE.md).

Le suivi de l'avancement est disponible dans [PROJECT_PROGRES.md](PROJECT_PROGRES.md). N'hésitez pas à le consulter et à le mettre à jour.

## Fonctionnalités Implémentées

Le projet en est à ses débuts. Les composants suivants sont disponibles :

### Gestion des utilisateurs

* Création et authentification d'utilisateurs avec rôles.

### Gestion des produits

* Ajout, modification et suppression de produits.

### Gestion des stocks

* Mises à jour d'inventaire (entrées et sorties).

### Gestion des ventes

* Enregistrement des ventes et mise à jour automatique du stock.
* Gestion des retours et remboursements.
* Impression de factures depuis le POS.
* Paiements numériques (cartes, mobile money, QR codes). Les transactions sont actuellement simulées.
* Rapports financiers détaillés et calcul des bénéfices.
* Tableaux de bord avec indicateurs clés (ventes, niveaux de stock).
  Une entrée de menu **Dashboard** permet maintenant d'ouvrir ce tableau de bord
  à tout moment.
* Prédiction de stock et alertes de seuil critique.
* Support multi‑utilisateurs connecté à une base de données partagée.
* Gestion de sessions utilisateur avec vérification des rôles lors des
  opérations sensibles (ajout de produits, mouvements de stock ou
  enregistrement des ventes).
* Interface multilingue basée sur les fichiers de traduction Qt.

## Planned Features

Les prochaines étapes concerneront l'amélioration de l'ergonomie et des tests automatisés.
* Support multi-utilisateurs complet en réseau.

## Architecture Technique

* Interface Qt moderne, ergonomique, et intuitive (Widgets/QML).
* Base de données MySQL optimisée pour la performance et la fiabilité.
* Séparation claire des responsabilités : UI, logique métier, couche de données.
* Configuration dynamique via fichier `config.ini`.

## Qualité du Code

* Structure claire, modulaire et réutilisable.
* Code commenté et directement exploitable avec Qt Creator.
* Gestion complète des erreurs et des transactions robustes.
* Optimisation poussée pour performances réseau.

## Sécurité

* Bonnes pratiques en sécurité : gestion des accès utilisateurs, protection des paiements.

## Exemple d’Intégration

* Exemple concret d’intégration Qt/C++ avec MySQL (connexion, requêtes, gestion des erreurs).

## Build and Run
Ensure Qt 5 (or later) with Widgets and SQL modules is installed.

```
mkdir build && cd build
cmake ..
make
./NieSApp
```

### REST API server

An optional HTTP server exposes product and sales data as a small REST API.
Build the `NieSApi` target and launch it. The listening port can be
specified with `--port` or the `NIES_REST_PORT` variable (default `8080`):

```bash
mkdir build && cd build
cmake ..
make NieSApi
./NieSApi --port 8080
```

Available endpoints:

- `GET /products` – list products
- `POST /products` – JSON body `{"name": ..., "price": ..., "discount": 0.0}`
- `GET /sales` – list recorded sales
- `POST /sales` – JSON body `{"product_id": ..., "quantity": ...}`


Before running, duplicate `config.example.ini` as `config.ini` and configure
your database parameters. `DatabaseManager` looks for this file next to the
executable by default. You may specify a custom location with the
`--config` option, with the `NIES_CONFIG_PATH` environment variable,
or by passing a path to the `DatabaseManager` constructor. You can also override individual
connection settings via environment variables:

- `NIES_DB_HOST` – database host
- `NIES_DB_PORT` – database port
- `NIES_DB_NAME` – database name
- `NIES_DB_USER` – database user
- `NIES_DB_PASSWORD` – database password
- `NIES_DB_DRIVER` – Qt SQL driver (e.g. `QMYSQL`, `QSQLITE`)
- `NIES_DB_OFFLINE_PATH` – path to the offline SQLite file
- `NIES_DB_BACKUP_PATH` – location of the JSON backup file
- `NIES_REST_PORT` – REST server port when running `NieSApi`
- `NIES_LANG` – override the UI language (e.g. `fr_FR`)
- `NIES_DASH_INTERVAL` – dashboard refresh interval in milliseconds

The dashboard update frequency can also be configured in `config.ini` under the
`[dashboard]` section with the key `update_interval` (default `60000`). The
`NIES_DASH_INTERVAL` environment variable overrides this value at runtime.

### Offline Mode

Set `offline=true` in `config.ini` (or `NIES_DB_OFFLINE=1`) to work with a local
SQLite file when the application cannot connect to MySQL. The file location is
controlled by `offline_path` or the `NIES_DB_OFFLINE_PATH` environment
variable. Call `DatabaseManager::synchronize()` once connectivity is restored to
push pending records to the MySQL server.
The application now includes a small `NetworkMonitor` helper that listens for
changes in network connectivity. When offline mode is enabled and the network
becomes available again, `DatabaseManager` automatically triggers
`synchronize()` so pending rows are uploaded without manual intervention.

`DatabaseManager` can also export a full backup of the current data set with
`exportBackup()` and reload it later using `restoreBackup()`. Set
`NIES_DB_BACKUP_PATH` to choose where this JSON backup is stored. Calling
`synchronize()` while online simply generates the backup file.

Example using environment variables:

```bash
NIES_DB_OFFLINE=1 NIES_DB_DRIVER=QSQLITE \
NIES_DB_OFFLINE_PATH=mydata.db ./NieSApp
```

This runs the application entirely with the local SQLite file `mydata.db`.

### Multilingual Support

The application loads translations from the `translations` directory next to the
executable. Use `lupdate` to update the `.ts` files and `cmake` will compile
them to `.qm` at build time. At runtime, `NieSApp` selects the translation that
matches your system locale by default. Set `language` in `config.ini` (section
`[app]`) or the `NIES_LANG` environment variable to override it.

To add a new language, duplicate `translations/NieS_fr.ts` with the desired
locale name such as `NieS_es.ts`. Translate the strings inside the
`<translation>` tags and run `lrelease` to produce the `.qm` file. Copy this
compiled file to the `translations` folder so the application can load it at
startup.

### Payment Processing (Simulated)

`PaymentProcessor` currently performs no real transactions. Calls to process
card, mobile money or QR code payments simply log the amount and report success
without contacting any external service. Integrate a payment gateway in this
class to accept actual payments.

### User Sessions and Permissions

Chaque client ouvre sa propre session via `LoginDialog`. Les actions des
gestionnaires s'appuient désormais sur la classe `UserSession` pour vérifier le
rôle courant. Seuls les administrateurs peuvent modifier les produits ou les
stocks, tandis que les vendeurs peuvent enregistrer des ventes. Plusieurs
instances de l'application peuvent ainsi se connecter simultanément à la même
base MySQL en toute sécurité.

## Running Tests

The project uses **Qt Test**. Build the test executable and run tests with
`ctest`:

```
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

This command executes the whole test suite and prints details when a test
fails.

Some tests previously started a temporary PostgreSQL server. These tests are
currently disabled. The rest of the suite runs against SQLite and does not
require a running database server.

## Database Initialization

The `database/schema` directory contains SQL scripts for creating the initial
tables (`users`, `products`, `inventory`, `sales` and `schema_migrations`). After creating your
MySQL database and editing `config.ini`, initialize the schema with:

```sh
mysql -u <your_user> <your_database> < database/schema/init.sql
```

Optional migration scripts live in `database/migrations`. You can apply them with
`database/migrate.sh`. The script relies on the `mysql` command line tool and
records each successfully applied file name in the `schema_migrations` table so
subsequent runs skip migrations that were already executed:

```sh
./database/migrate.sh <your_database> <your_user>
```

To populate your fresh database with a small demo dataset, run:

```sh
mysql -u <your_user> <your_database> < database/seed.sql
```

This creates an administrator account (`admin`/`adminpass`) and a couple of
example products so you can explore the application right away.

## Troubleshooting

If the application fails to connect to MySQL, check the following:

1. **Configuration file available** – copy `config.example.ini` to `config.ini`
   next to the executable (or the path specified via `--config` or
   `NIES_CONFIG_PATH`) and
   fill in actual values for `name`, `user` and `password`.
2. **Environment overrides** – The variables `NIES_DB_HOST`, `NIES_DB_PORT`,
   `NIES_DB_NAME`, `NIES_DB_USER` and `NIES_DB_PASSWORD` can override settings at
   runtime. Ensure they are correct or unset.
3. **MySQL service running** – Verify your MySQL server is up and listening on
   the configured host and port (e.g. `localhost:3306`).
4. **Qt MySQL driver installed** – If you see “`QMYSQL driver not loaded`”, make
   sure Qt's MySQL plugin (`qsqlmysql.dll` or `libqsqlmysql.so`) and the
   corresponding `libmysql` client library are available in the search path.

## License

This project is licensed under the [MIT License](LICENSE).

