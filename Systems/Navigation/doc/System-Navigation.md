[README](../../../README.md)


- [System: Navigation](#system-navigation)
- [Overview](#overview)
  - [Purpose](#purpose)
  - [General Requirements](#general-requirements)
- [System Architecture](#system-architecture)
- [Inputs](#inputs)
- [Outputs](#outputs)
- [How It Works](#how-it-works)
  - [Detailed Documentation](#detailed-documentation)
  - [Software Content](#software-content)
- [Subsystems](#subsystems)
  - [Package Diagram](#package-diagram)
- [Usage Instructions](#usage-instructions)
- [Validation](#validation)


# System: Navigation

# Overview

## Purpose

The Navigation System's role in the Robot Framework is to drive a mobile base.

## General Requirements

# System Architecture

# Inputs

The following inputs are required in order for this system to properly function.

| Input | DataType | Description | Requirement |
| ----- | -------- | ----------- | ----------- |

# Outputs

The following outputs are provided by this system.

| Output | DataType | Description | Usage |
| ------ | -------- | ----------- | ----- |

# How It Works

## Detailed Documentation

## Software Content

# Subsystems

The following Subsystems are provided in this System:

| State | Subsystem                                                                                   | Purpose                                                                                                              |
| ----- | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| NEW   | [Navigation Executor](../Subsystems/NavigationExecutor/doc/Subsystem-NavigationExecutor.md) | Given Drive Commands, will generae Base Machine commands suitable for some Base Machine component to move the robot. |

## Package Diagram

![](puml/SystemNavigationPackageDiagram.png)

# Usage Instructions

# Validation

