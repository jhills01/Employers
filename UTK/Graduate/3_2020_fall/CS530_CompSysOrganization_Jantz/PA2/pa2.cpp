#include "pa2.h"

//checks if circular queue is full
bool pa2::cQueue::isFull()
{
    if (front == -1) {return false;} //circular queue is empty, regardless of size
    else if (size == 1 && front != -1) return true;//required check b/c "(front-1)%(size-1)" below throws err when size is 1
    else if ( (front == 0 && rear == size-1) || (rear == front-1) ) {return true;}
    else {return false;}
}

// Insert element in back of circular queue
// returns robEntryNum if "e" was inserted, or -1 if insertion failed b/c queue is full
int pa2::cQueue::pushBack(ROBentry *e) 
{ 
    //Queue is full - insertion has failed. Return -1
    if (size == 1 && front != -1) return -1;
    else if ( (front == 0 && rear == size-1) || (rear == front-1) ) {return -1;}
  
    // Insert First Element  
    else if (front == -1) 
    { 
        front = rear = 0; 
        cqArray[rear] = e;
    } 
  
    // loop around and insert new element in the beginning element of array if it is not "front"
    else if (rear == size-1 && front != 0)
    { 
        rear = 0; 
        cqArray[rear] = e;
    } 
  
    // normal insert without looping to beginning of array
    else
    { 
        rear++; 
        cqArray[rear] = e;
    } 

    //insertion successful
    return rear;
} 
  
// Deletes 1st element from circular queue
// returns deleted element or NULL if cQueue is empty
pa2::ROBentry * pa2::cQueue::popFront() 
{ 
    //cQueue is empty
    if (front == -1) 
    { 
        return NULL; 
    } 
  
    ROBentry *e = cqArray[front]; 
    cqArray[front] = NULL; 
    
    //adjust circular queue
    //cQueue is now empty
    if (front == rear) 
    { 
        front = -1; 
        rear = -1; 
    } 
    
    //new front element loops back around to the beginning of cQueue cqArray
    else if (front == size-1) front = 0; 
    
    //new front element is next cqArray element 
    else front++; 
  
    return e; 
} 
  