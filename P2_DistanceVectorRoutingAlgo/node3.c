/////
#include <stdio.h>
#include "dv.h"
#include <stdlib.h>

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt3;
struct NeighborCosts   *neighbor3;

static int myNodeNum = 3;
//////////////////////
// Helper Functions //
//////////////////////
void static printToConsole(int node, int dest){
    printf("At time t=%f, node %d sends packet to node %d with:  ", clocktime, node, dest);
    for(int i = 0; i < neighbor3->NodesInNetwork; i++){
        printf("%d  ", neighbor3->NodeCosts[i]);
    }
    printf("\n");
}

/* students to write the following two routines, and maybe some others */

void rtinit3() {
    // test for bo chen
     //   printf("node 3 initial time: %f\n", clocktime);
    printf("\nAt time t=%f rtinit3() called\n", clocktime);
    struct RoutePacket packet;
    packet.sourceid = 3;
    neighbor3 = getNeighborCosts(3);
    for(int i = 0; i < neighbor3->NodesInNetwork; i++){ //Node number from 0 to MAX_NODES
        for(int j = 0; j < neighbor3->NodesInNetwork; j++){
            if(i == j){
                dt3.costs[i][j] = neighbor3->NodeCosts[i];
            }else{
                dt3.costs[i][j] = INFINITY;
            }
            packet.mincost[i] = neighbor3->NodeCosts[i];
            // if(packet.mincost[i] > dt3.costs[i][j]){
            //     packet.mincost[i] = dt3.costs[i][j];
            // }
        }
    }
    printf("Entire Node in Network is %d\n", neighbor3->NodesInNetwork);
    printdt3(myNodeNum, neighbor3, &dt3);
    
    //All links should be bi-directional and the costs in both directions are identical
    //Might have to initialize other data structures 
    //Send the minimum cost paths to directly-connected neighbors by calling tolayer2().
    int i;
    int updateOccur = 0;
    int TotalNodes = neighbor3->NodesInNetwork;     // Total nodes in network
    int NumberOfNeighbors = 0;  //was 0          // How many neighbors
    int Neighbors[MAX_NODES];                      // Who are the neighbors
    // Determine our neighbors 
    for (i = 0; i < TotalNodes; i++ )  {
        if (( neighbor3->NodeCosts[i] != INFINITY ) && i != myNodeNum )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    for ( i = 0; i < NumberOfNeighbors; i++ ){
        int neighbor = Neighbors[i];
        if ( neighbor != myNodeNum )  {//Send to direct neighbor
            packet.destid = neighbor;
            toLayer2(packet);
            printToConsole(myNodeNum, neighbor);
        }
    }
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
    //uses these received values to update its own distance table 
    printf("Node 3 Received: \n");
    printf("Sender SourceID: %d, DestId %d\n", rcvdpkt->sourceid, rcvdpkt->destid);
    int i;
    int updateOccur = 0;
    
    struct RoutePacket *mypktptr = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    mypktptr->sourceid = myNodeNum;
    
    //If its own minimum cost to another node changes as a result
    //of the update, node 0 informs its directly connected neighbors of this change in minimum
    //cost by sending them a routing packet
    int TotalNodes = neighbor3->NodesInNetwork;     // Total nodes in network
    int NumberOfNeighbors = 0;                      // How many neighbors
    int Neighbors[MAX_NODES];                      // Who are the neighbors
    // Determine our neighbors 
    for (i = 0; i < TotalNodes; i++ )  {
        if (( neighbor3->NodeCosts[i] != INFINITY ) && i != myNodeNum )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }


    // for(i = 0; i < neighbor3->NodesInNetwork; i++){
    //     if(dt3.costs[rcvdpkt->sourceid][i] > rcvdpkt->mincost[i]  && rcvdpkt->mincost[i] != 0){
    //         updateOccur = 1;
    //         dt3.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i]; 
    //         mypktptr->mincost[i] = dt3.costs[rcvdpkt->sourceid][i];
    //     }
    // }
    
    //  for(i = 0; i < NumberOfNeighbors; i++){
    //     int neighbor = Neighbors[i]; //neighbors number
    //     //If the current node's entry costs is greater than the packet's mincost then we do some updates to the table. 
    //     printf("dt3.cost: %d \t rcvdpkt->minCost: %d ", dt3.costs[rcvdpkt->sourceid][i],rcvdpkt->mincost[i]);
        
    //     //Find the distance from neighbor rcvdpkt->sourceid
    //     int firstPath = dt3.costs[rcvdpkt->sourceid][i];
    //     //Fidn path from neighbors table to the i value. 
    //     int secondPath = rcvdpkt->mincost[i];
    //     //Sum the new path
    //     int summedPath = firstPath + secondPath;

    //     printf("Summed to %d\n", summedPath);
    //     if( dt3.costs[rcvdpkt->sourceid][i] > rcvdpkt->mincost[i] && rcvdpkt->mincost[i] != 0 && summedPath > 0  ){
    //         updateOccur = 1;
    //         dt3.costs[rcvdpkt->sourceid][i] = summedPath;
    //         mypktptr->mincost[i] = summedPath;
    //     }
    //     //printf("DistTable: %d Mincost %d\n", dt3.costs[rcvdpkt->sourceid][i], rcvdpkt->mincost[i]);
    //     //Didn't update its own distance table entry
    // }

    printdt3(myNodeNum, neighbor3, &dt3);

    if(updateOccur){
        printf("Update Occur Informing Neighbor\n");
        for ( i = 0; i < NumberOfNeighbors; i++ ){
        int neighbor = Neighbors[i];
        if ( neighbor != myNodeNum )  {//Send to direct neighbor
                mypktptr->destid = neighbor;
                toLayer2(*mypktptr);
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt3

