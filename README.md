# Application de Gestion Innovante pour Boutique ou Restaurant

## Description

Ce projet consiste à créer une application moderne et innovante de gestion de boutique ou de restaurant en utilisant les technologies Qt (C++ avec Qt Widgets/QML) et PostgreSQL.

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
* Base de données PostgreSQL optimisée pour la performance et la fiabilité.
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

* Exemple concret d’intégration Qt/C++ avec PostgreSQL (connexion, requêtes, gestion des erreurs).

## Build and Run
Ensure Qt 5 (or later) with Widgets and SQL modules is installed.

```
mkdir build && cd build
cmake ..
make
./NieSApp
```

Configure database parameters in `config.ini` before running. `DatabaseManager`
looks for this file next to the executable by default. You may specify a custom
location with the `NIES_CONFIG_PATH` environment variable or by passing a path
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

Some tests start a temporary PostgreSQL server. By default the test suite
expects `initdb` and `pg_ctl` in `/usr/lib/postgresql/16/bin`. Set the
`NIES_INITDB_PATH` and `NIES_PG_CTL_PATH` environment variables to override
these paths. If you cannot run PostgreSQL locally (for example because you lack
root access), set `NIES_SKIP_PG_TESTS=1` to skip the PostgreSQL dependent tests.

## Database Initialization

The `database/schema` directory contains SQL scripts for creating the initial
tables (`users`, `products`, `inventory` and `sales`). After creating your
PostgreSQL database and editing `config.ini`, initialize the schema with:

```sh
psql -U <your_user> -d <your_database> -f database/schema/init.sql
```

Optional migration scripts live in `database/migrations`. You can apply them with
`database/migrate.sh`:

```sh
./database/migrate.sh <your_database> <your_user>
```

## License

This project is licensed under the [MIT License](LICENSE).

