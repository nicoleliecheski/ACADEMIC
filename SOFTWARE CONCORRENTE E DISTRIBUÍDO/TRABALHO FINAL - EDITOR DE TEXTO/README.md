# Course Final Project Design and Implementation of a Concurrent and Distributed Software System

## Objective
The objective of the project is to exercise, in an integrated manner, the concepts of distributed systems and concurrent programming in the construction of a software system. The project must explore methods and patterns to solve the main problems of concurrency and distribution, making use of currently relevant technologies and tools.

## Overview
The system to be developed must contain elements of distributed systems and concurrent programming, involving different programming models (with the use of more than one programming language) and interaction paradigms (client-server, publish-subscribe, messaging).

**The system must contain the following characteristics, regardless of the application scenario:**
- **Service accessible to multiple clients on the Internet;**
- **Service constituted through the integration and coordination of several distributed components, which must be implemented as part of the project;**
- **Concurrent accesses to shared resources/data;**
- **Data processing on the server side, concurrently with client accesses;**
- **Use of synchronous (blocking) and asynchronous remote interaction mechanisms;**
- **Replication and partitioning of data and functionalities;**
- **Treatments to ensure data consistency and availability of functionalities.**

The elaboration of the specific application scenario, including requirements and architecture, is an integral part of the project.

## Examples of Application Scenarios
- Shared database, which can be accessed simultaneously by different remote clients, with operations for data access (minimally, CRUD), event notification (e.g., related to performance parameters) to an administration dashboard, and automatic maintenance operations (e.g., sorting, data sanitization, consistency, etc.).
- **Shared document editor, with functionalities for remote visualization and editing by several simultaneous clients, notification of editing events to the other clients, and background processing operations (e.g., spell checker, text formatter).**
- Multiplayer online game, in which multiple players can simultaneously view the shared state of the game, execute actions that modify this state, and receive notifications of state changes made by other players or by internal game rule maintenance operations.
- Inventory tracking system, in which multiple sellers and buyers can simultaneously perform checkout (sale) or check-in (purchase) operations of products, with support for alerts (e.g., low quantity products) and for internal inventory maintenance operations (e.g., for quantity reconciliation after losses).

## Format
- **Implementation of the system services with the characteristics described above, along with simulated clients to demonstrate their use.**
- **Create a representative demonstration scenario that allows systematically exercising the system's characteristics.**
- **Execute the demonstration using the AWS cloud (EC2).**
- **Artifacts to be delivered: source code (and executables); documentation (of architecture and implementation); usage instructions (readme); and test data.**
