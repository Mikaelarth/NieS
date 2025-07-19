# Cahier des Charges

## Objectifs

Concevoir une application de gestion moderne pour boutique ou restaurant. Elle doit faciliter le suivi des produits, des ventes et des utilisateurs tout en restant fiable et sécurisée.

## Modules de base

- **Gestion des utilisateurs** : création de comptes, authentification et rôles.
- **Gestion des produits** : ajout et modification du catalogue.
- **Gestion des stocks** : mouvements d'inventaire et alertes de seuil.
- **Gestion des ventes** : enregistrement des transactions et calcul automatique des totaux.

## Fonctionnalités avancées

- Support multi‑utilisateur avec permissions.
- Intégration des paiements numériques (cartes, mobile money, QR code).
- Tableaux de bord et indicateurs clés de performance.
- Prédiction de stock et de ventes via analyse des données.
- Sauvegarde et synchronisation de la base de données.
- Mode hors ligne avec synchronisation ultérieure.

## Aspects techniques

- Base de données **MySQL** avec scripts de migration.
- Interface **Qt** (Widgets ou QML) pour le poste de travail et terminaux mobiles.
- Architecture modulaire séparant interface, logique métier et accès aux données.
- Sécurité renforcée : mots de passe hachés et contrôle des accès.
- Stratégie de tests automatisés à l'aide de **Qt Test**.

## Roadmap

1. **Fondations** – mise en place de l'architecture, du dépôt et des migrations MySQL.
2. **Fonctionnalités principales** – implémentation des modules utilisateurs, produits, stock et ventes.
3. **Avancées technologiques** – ajout des permissions avancées, des paiements numériques, des tableaux de bord, des prédictions, du mode hors ligne et des mécanismes de backup/synchronisation.
4. **Finition et déploiement** – peaufinage de l'interface, tests approfondis et déploiement sur les différentes plateformes.
