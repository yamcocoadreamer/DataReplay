# DataReplay Project

## Overview

DataReplay est un système de visualisation et de lecture de trajectoires développé avec Unreal Engine 5.6.
Le projet permet d’importer des données de trajectoire depuis des fichiers CSV et de les rejouer à l’aide de contrôles interactifs avancés.

---

## Features

### Data Import

- Import de fichiers CSV via DataTable
- Structure de données générique et extensible (`FDroneWaypointRow`)
- Support de trajectoires temporelles (Time, X, Y, Z)

### Playback Controls

- Play : démarrer la lecture
- Pause : mettre en pause
- Stop : arrêter et revenir au début
- Timeline Slider : navigation temporelle manuelle
- Vitesse ajustable : lecture accélérée ou ralentie (0.1x à 10x)
- Lecture en boucle
- Lecture inverse

### Visualization

- Affichage de la trajectoire sous forme de ligne suivant l’ensemble du parcours
- Activation / désactivation de l’affichage de la trajectoire
- Couleur de trajectoire personnalisable
- Épaisseur ajustable (1 à 20)
- Waypoints optionnels affichés sous forme de sphères
- Interpolation linéaire pour un mouvement fluide

### User Interface (UMG)

- Widget de contrôle dédié
- Affichage du temps courant et du temps total
- Mise à jour en temps réel
- Texte des boutons dynamique

---

## Project Architecture

### C++ File Structure

Source/DataReplay/
├── DroneWaypointStruct.h/.cpp # Structure de données CSV
├── TrajectoryReplayActor.h/.cpp # Actor principal de replay
└── ReplayControlWidget.h/.cpp # Widget UI de contrôle


### Core Classes

#### FDroneWaypointRow

Structure de données représentant un point de trajectoire :

- Time : temps en secondes
- X, Y, Z : coordonnées spatiales

#### ATrajectoryReplayActor

Actor responsable de la lecture et de la visualisation :

- Chargement des données depuis une DataTable
- Interpolation de la position
- Visualisation de la trajectoire
- Gestion des contrôles de lecture

#### UReplayControlWidget

Widget UMG pour l’interface utilisateur :

- Boutons de contrôle
- Slider de timeline
- Affichage du temps
- Communication bidirectionnelle avec l’Actor

---

## CSV File Format

---,Time,X,Y,Z
Row0,0,0,0,100
Row1,1,200,0,100
Row2,2,400,100,150
...


### Important Notes

- La première colonne contient les identifiants de ligne (Row0, Row1, etc.)
- La première cellule doit obligatoirement être `---`
- Les colonnes doivent être : Time, X, Y, Z

---

## Installation and Usage

### 1. Create the Project

- Créer un nouveau projet C++ dans Unreal Engine 5.6
- Template : Blank
- Nom du projet : DataReplay

### 2. Add C++ Files

- Copier les fichiers `.h` et `.cpp` dans :
Source/DataReplay/
- Compiler le projet dans Visual Studio

### 3. Import CSV Data

- Content Browser → Import
- Sélectionner le fichier CSV
- Choisir `FDroneWaypointRow` comme structure
- Créer la DataTable

### 4. Scene Setup

- Placer `TrajectoryReplayActor` dans le niveau
- Assigner la DataTable dans les propriétés de l’Actor
- Ajouter un mesh de visualisation (optionnel)

### 5. Create the UI

- Créer `WBP_ReplayControls` (Widget Blueprint)
- Ajouter les widgets : Buttons, Slider, TextBlock
- Définir `ReplayControlWidget` comme classe parente
- Placer l’UI dans le niveau via `BP_UIManager`

---

## Configuration

### Playback Settings

- Playback Speed (défaut : 1.0)
- Auto Play
- Loop Playback
- Reverse Playback

### Visualization Settings

- Show Trajectory
- Trajectory Color (défaut : vert)
- Trajectory Thickness (défaut : 5.0)
- Show Waypoints
- Waypoint Radius (défaut : 10.0)

---

## Customization and Extensibility

### Suggested Colors

- Vert laser : (0, 1, 0, 1)
- Bleu électrique : (0, 0.5, 1, 1)
- Orange : (1, 0.5, 0, 1)
- Cyan : (0, 1, 1, 1)

### System Extension

Le système est conçu pour être extensible :

- Remplacer `FDroneWaypointRow` par une structure personnalisée
- Ajouter des paramètres supplémentaires (vitesse, rotation, métadonnées)
- Modifier la logique d’interpolation
- Ajouter des événements Blueprint

---

## Troubleshooting

### CSV Import Fails

- Vérifier que la première ligne est : `---,Time,X,Y,Z`
- Compiler le code avant l’import
- Vérifier que `FDroneWaypointRow` est bien disponible

### Actor Does Not Move

- Vérifier que la DataTable est assignée
- Vérifier que la lecture est activée
- Consulter l’Output Log

### UI Does Not Appear

- Vérifier que `BP_UIManager` est présent dans le niveau
- Vérifier les noms des widgets (`BindWidget`)
- Compiler le Widget Blueprint

### Trajectory Is Not Visible

- Activer l’option Show Trajectory
- Vérifier que des données sont chargées
- Augmenter la valeur de Trajectory Thickness

---

## Use Cases

- Simulation de drones (replay de missions)
- Analyse de parcours pour véhicules autonomes
- Prévisualisation de mouvements de caméra (cinématiques)
- Formation et démonstration
- Visualisation de données GPS

---

## Future Improvements

- Support de formats supplémentaires (JSON, XML)
- Interpolation courbe (Bezier, spline)
- Export de trajectoires modifiées
- Lecture synchronisée multi-acteurs
- Enregistrement en temps réel
- Marqueurs temporels
- Annotations sur la trajectoire

---

## Author

Projet développé pour démontrer des compétences en Unreal Engine C++ et UMG.

---

## License

Projet de démonstration – libre d’utilisation.

Version : 1.0  
Unreal Engine : 5.6  
Langage : C++17  
Date : 2026
