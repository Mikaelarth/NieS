# Application de Gestion Innovante pour Boutique ou Restaurant

## Description

Ce projet consiste à créer une application moderne et innovante de gestion de boutique ou de restaurant en utilisant les technologies Qt (C++ avec Qt Widgets/QML) et PostgreSQL.

## Fonctionnalités Principales

### 1. Gestion des Ventes

* Interface intuitive pour un Point de Vente (POS) rapide.
* Gestion complète des retours et remboursements.
* Impression automatisée des factures.

### 2. Gestion des Stocks

* Entrées et sorties automatiques.
* Inventaire mis à jour en temps réel.
* Alertes configurables pour seuil critique.
* Option de gestion prédictive basée sur l'intelligence artificielle.

### 3. Gestion Financière

* Rapports quotidiens, hebdomadaires et mensuels.
* Suivi détaillé des dépenses et revenus.
* Calcul automatique des bénéfices nets.

### 4. Paiements Intégrés

* Paiements numériques intégrés (cartes, mobile money, QR codes).
* Sécurité renforcée des transactions financières.

### 5. Gestion des Utilisateurs

* Système robuste d’authentification.
* Niveaux d’accès multiples : Administrateur, gestionnaire, vendeur.

### 6. Tableaux de Bord Interactifs

* Visualisation dynamique en temps réel.
* Indicateurs clés de performances (ventes journalières, bénéfices, alertes de stock).

### 7. Support Multi-utilisateurs en Réseau

* Utilisation simultanée par plusieurs utilisateurs.
* Transactions sécurisées et optimisées via PostgreSQL.

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

Configure database parameters in `config.ini` before running. You can also
override any of the connection settings via environment variables:

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

