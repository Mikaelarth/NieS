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

## Planned Features

Les fonctionnalités suivantes sont prévues mais **ne sont pas encore implémentées** :

* Interface POS complète avec retours, remboursements et impression des factures.
* Alertes de seuil critique et prédiction de stock basée sur l'intelligence artificielle.
* Rapports financiers détaillés et calcul automatique des bénéfices.
* Paiements numériques intégrés (cartes, mobile money, QR codes).
* Tableaux de bord interactifs et indicateurs clés de performances.
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

Before running, duplicate `config.example.ini` as `config.ini` and configure
your database parameters. `DatabaseManager` looks for this file next to the
executable by default. You may specify a custom location with the
`NIES_CONFIG_PATH` environment variable or by passing a path
to the `DatabaseManager` constructor. You can also override individual
connection settings via environment variables:

- `NIES_DB_HOST` – database host
- `NIES_DB_PORT` – database port
- `NIES_DB_NAME` – database name
- `NIES_DB_USER` – database user
- `NIES_DB_PASSWORD` – database password

## Running Tests

The project uses **Qt Test**. Build the test executable and run tests with
`ctest`:

```
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

Some tests previously started a temporary PostgreSQL server. These tests are
currently disabled. The rest of the suite runs against SQLite and does not
require a running database server.

## Database Initialization

The `database/schema` directory contains SQL scripts for creating the initial
tables (`users`, `products`, `inventory` and `sales`). After creating your
MySQL database and editing `config.ini`, initialize the schema with:

```sh
mysql -u <your_user> <your_database> < database/schema/init.sql
```

Optional migration scripts live in `database/migrations`. You can apply them with
`database/migrate.sh` which relies on the `mysql` command line tool:

```sh
./database/migrate.sh <your_database> <your_user>
```

## Troubleshooting

If the application fails to connect to MySQL, check the following:

1. **Configuration file available** – copy `config.example.ini` to `config.ini`
   next to the executable (or the path specified via `NIES_CONFIG_PATH`) and
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

