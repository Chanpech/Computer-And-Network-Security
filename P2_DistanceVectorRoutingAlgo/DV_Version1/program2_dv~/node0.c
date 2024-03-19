/////Author: Chanpech Hoeng
/////Date: 12/4/2023
#include <stdio.h>
#include "dv.h"
#include <stdlib.h>

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt0;
struct NeighborCosts   *neighbor0;

static int myNodeNum = 0;
// Part of the assignment asks that you write node4.c and node5.c so that you
// end up with a total of 6 nodes.  But your startoff code only has
// 4 nodes; node0.c, node1.c, node2.c, and node3.c
// So here are the stubs for routines that could appear in
// node4.c and node5.c should you choose to write them.  If you do so,
// you should eliminate the entries here.
void    rtinit4(){ }
void    rtinit5(){ }
void    rtinit6(){ }
void    rtinit7(){ }
void    rtinit8(){ }
void    rtinit9(){ }
void    rtupdate4( struct RoutePacket *rcvdpkt ) { }
void    rtupdate5( struct RoutePacket *rcvdpkt ) { }
void    rtupdate6( struct RoutePacket *rcvdpkt ) { }
void    rtupdate7( struct RoutePacket *rcvdpkt ) { }
void    rtupdate8( struct RoutePacket *rcvdpkt ) { }
void    rtupdate9( struct RoutePacket *rcvdpkt ) { }

// void informNeighbors(int destination){
//     struct RoutePacket *mypktptr = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
//     mypktptr->sourceid = 0;
//     mypktptr->destid = destination;
//     mypktptr->mincost = dt0.costs;
// }

void static printToConsole(int node, int dest){
    printf("At time t=%f, node %d sends packet to node %d with:  ", clocktime, node, dest);
    for(int i = 0; i < neighbor0->NodesInNetwork; i++){
        printf("%d  ", neighbor0->NodeCosts[i]);
    }
    printf("\n");
}


// int getNumberOfNeighbors(){
//     int length = sizeof(MAX_NODES-1)
//     return ;
// }

/* students to write the following two routines, and maybe some others */
void rtinit0() {
    //printf("node 0 initial time: %f\n", clocktime);
    printf("At time t=%f rtinit0() called\n", clocktime);
    struct RoutePacket packet;
    packet.sourceid = 0;
    neighbor0 = getNeighborCosts(0);
    for(int i = 0; i < neighbor0->NodesInNetwork; i++){ //Node number from 0 to MAX_NODES
        for(int j = 0; j < neighbor0->NodesInNetwork; j++){
            if(i == j){
                dt0.costs[i][j] = neighbor0->NodeCosts[i];
            }else{
                dt0.costs[i][j] = INFINITY;
            }
            //packet.mincost[i] = dt0.costs[i][j];
            packet.mincost[i] = neighbor0->NodeCosts[i];
        }
    }
    printf("Entire Node in Network is %d\n", neighbor0->NodesInNetwork);
    printdt0(0, neighbor0, &dt0);
    
    //All links should be bi-directional and the costs in both directions are identical
    //Might have to initialize other data structures 
    //Send the minimum cost paths to directly-connected neighbors by calling tolayer2().
    int i;
    int updateOccur = 0;
    int TotalNodes = neighbor0->NodesInNetwork;     // Total nodes in network
    int NumberOfNeighbors = 0;  //was 0          // How many neighbors
    int Neighbors[MAX_NODES];                      // Who are the neighbors
    // Determine our neighbors 
    for (i = 0; i < TotalNodes; i++ )  {
        if (( neighbor0->NodeCosts[i] != INFINITY ) && i != 0 )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    for ( i = 0; i < NumberOfNeighbors; i++ ){
        int neighbor = Neighbors[i];
        if ( neighbor != myNodeNum )  {//Send to direct neighbor
            printf("dest %d ", neighbor);
            packet.destid = neighbor;
            toLayer2(packet);
            printToConsole(myNodeNum, neighbor);
        }
    }
}

/**
 * Called once node receives a routing packet that was sent to it by one of its directly connected neighors.
 * 
 * @param *rcvdpkt is a pointer to the packet that was received.
 * contain i’s current shortest path costs to all other network nodes.
 * */

// 1) Print the identity of the sender of the routing packet that is being passed to your routine,
// and
// 2) Whether or not the distance table is updated, print the content of the distance table
// (e.g., use print routine available in the template), and
// 3) A description of any messages sent to neighboring nodes as a result of any distance table
// updates
void rtupdate0( struct RoutePacket *rcvdpkt ) {
    //uses these received values to update its own distance table 
    printf("Node 0 Received: \n");
    printf("Sender SourceID: %d, DestId %d\n", rcvdpkt->sourceid, rcvdpkt->destid);
    int i;
    int updateOccur = 0;
    
    struct RoutePacket *mypktptr = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    mypktptr->sourceid = 0;
    
    //If its own minimum cost to another node changes as a result
    //of the update, node 0 informs its directly connected neighbors of this change in minimum
    //cost by sending them a routing packet
    int TotalNodes = neighbor0->NodesInNetwork;     // Total nodes in network
    int NumberOfNeighbors = 0;                      // How many neighbors
    int Neighbors[MAX_NODES];                      // Who are the neighbors
    // Determine our neighbors 
    for (i = 0; i < TotalNodes; i++ )  {
        if (( neighbor0->NodeCosts[i] != INFINITY ) && i != myNodeNum )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    /*
   D0 |     1     2     3
  ----|-------------------------------
dest 1|     1  9999  9999
dest 2|  9999     3  9999
dest 3|  9999  9999     7
    */
   //dt0 is this node's distance table
   //while neighbor0 is 
   /*
   struct NeighborCosts {
    int NodesInNetwork; // The total number of nodes in the entire network
    int NodeCosts[MAX NODES]; // An array of cost
    }*/
    
    for(i = 0; i < neighbor0->NodesInNetwork; i++){
        //printf("Before DistTable: %d\n", dt0.costs[rcvdpkt->sourceid][i]);
        if(dt0.costs[rcvdpkt->sourceid][i] > rcvdpkt->mincost[i]  && rcvdpkt->mincost[i] != 0){
            //Use the sourceid as the identifier of rows.
            //Then set each entry to the respected routePacket's mincost.
            updateOccur = 1;
            dt0.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i]; 
            mypktptr->mincost[i] = dt0.costs[rcvdpkt->sourceid][i];
        }
    }
    
    // for(i = 0; i < NumberOfNeighbors; i++){
    //     int neighbor = Neighbors[i]; //neighbors number
    //     //If the current node's entry costs is greater than the packet's mincost then we do some updates to the table. 
    //     printf("dt0.cost: %d \t rcvdpkt->minCost: %d ", dt0.costs[rcvdpkt->sourceid][i],rcvdpkt->mincost[i]);
        
    //     //Find the distance from neighbor rcvdpkt->sourceid
    //     int firstPath = dt0.costs[rcvdpkt->sourceid][i];
    //     //Fidn path from neighbors table to the i value. 
    //     int secondPath = rcvdpkt->mincost[i];
    //     //Sum the new path
    //     int summedPath = firstPath + secondPath;

    //     printf("Summed to %d\n", summedPath);
    //     if( dt0.costs[rcvdpkt->sourceid][i] > rcvdpkt->mincost[i] && rcvdpkt->mincost[i] != 0){
    //         updateOccur = 1;
    //         dt0.costs[rcvdpkt->sourceid][i] = summedPath;
    //         mypktptr->mincost[i] = summedPath;
    //     }
    //     //printf("DistTable: %d Mincost %d\n", dt0.costs[rcvdpkt->sourceid][i], rcvdpkt->mincost[i]);
    //     //Didn't update its own distance table entry
    // }

    printdt0(myNodeNum, neighbor0, &dt0);

   
    if(updateOccur){
        printf("Update Occur Informing Neighbor\n");
        for ( i = 0; i < NumberOfNeighbors; i++ ){
            int neighbor = Neighbors[i];
            //printf("dest %d ", neighbor);
            if ( neighbor != myNodeNum )  {//Send to direct neighbor
                printf("dest %d ", neighbor);
                mypktptr->destid = neighbor;
                toLayer2(*mypktptr);
                printToConsole(myNodeNum, neighbor);
            }
        }
    }
    //printf("node 0 initial time: %f\n", clocktime);
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
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt0

