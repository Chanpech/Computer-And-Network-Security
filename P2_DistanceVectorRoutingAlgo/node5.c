/////
#include <stdio.h>
#include "dv.h"
#include <stdlib.h>

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt5;
struct NeighborCosts   *neighbor5;

static int myNodeNum = 5;
/* students to write the following two routines, and maybe some others */

void rtinit4() {
    // test for bo chen
     //   printf("node 5 initial time: %f\n", clocktime);
      printf("At time t=%f rtinit5() called\n", clocktime);
    struct RoutePacket packet;
    packet.sourceid = 4;
    //UNREACHABLE
    neighbor5 = getNeighborCosts(5);
    for(int i = 0; i < neighbor5->NodesInNetwork; i++){ //Node number from 0 to MAX_NODES
        for(int j = 0; j < neighbor5->NodesInNetwork; j++){
            if(i == j){
                dt5.costs[i][j] = neighbor5->NodeCosts[i];
            }else{
                dt5.costs[i][j] = INFINITY;
            }
            if(packet.mincost[i] > dt5.costs[i][j]){
                packet.mincost[i] = dt5.costs[i][j];
            }
            //printf("[%d][%d]=%d ", i, j,  dt5.costs[i][j]);
        }
    }
    printf("Entire Node in Network is %d\n", neighbor5->NodesInNetwork);
    printdt5(0, neighbor5, &dt5);
    
    //All links should be bi-directional and the costs in both directions are identical
    //Might have to initialize other data structures 
    //Send the minimum cost paths to directly-connected neighbors by calling tolayer2().
    int i;
    int updateOccur = 0;
    int TotalNodes = neighbor5->NodesInNetwork;     // Total nodes in network
    int NumberOfNeighbors = 0;  //was 0          // How many neighbors
    int Neighbors[MAX_NODES];                      // Who are the neighbors
    // Determine our neighbors 
    for (i = 0; i < TotalNodes; i++ )  {
        if (( neighbor5->NodeCosts[i] != INFINITY ) && i != 0 )  {
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


void rtupdate5( struct RoutePacket *rcvdpkt ) {

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
void printdt5( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt5

