#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "presentation.h"


// Make a copy of the queue
void presentationQueue_duplicate(tPresentationQueue* dst, tPresentationQueue src) {
    
    tPresentationQueueNode *pNode;
    
    // Check preconditions
    assert(dst!=NULL);
    
    // Initialize the new queue
    presentationQueue_createQueue(dst);
    
    // Assign pointer to first element    
    pNode = src.first;
    while(pNode != NULL) {
        // Enqueue the current element to the output queue
        presentationQueue_enqueue(dst, pNode->e);
        
        // Move to next element
        pNode = pNode->next;
    }    
}


// Implementation of compare method between two presentations queues using iterative algorithm
bool presentationQueue_compareIterative(tPresentationQueue *q1, tPresentationQueue *q2) {
    bool equals = true;
    tPresentation *p1, *p2;       
    
    // While the queues have elements and all previous elements were equal, check for next elements    
    while(equals && !presentationQueue_empty(*q1) && !presentationQueue_empty(*q2)) {
        // Check if one of the queues is empty
        if( (presentationQueue_empty(*q1) && !presentationQueue_empty(*q2)) || (!presentationQueue_empty(*q1) && presentationQueue_empty(*q2)) ) {
            // Queues have different number of elements
            equals = false;
        } else {
            // Both queues have elements. 
            // Take head elements
            p1 = presentationQueue_head(*q1);
            p2 = presentationQueue_head(*q2);
            
            // Check that elements are not NULL
            assert(p1 != NULL);
            assert(p2 != NULL);
            
            // Compare the headers
            if(!presentation_compare(*p1, *p2)) {                
                // The elements are different.
                equals = false;
            }
            
            // Dequeue the head elements from queues
            presentationQueue_dequeue(q1);
            presentationQueue_dequeue(q2);
        }        
    }
    
    return equals;
}


// Implementation of compare method between two presentation queues using recursive algorithm
bool presentationQueue_compareRecursive(tPresentationQueue *q1, tPresentationQueue *q2) {
    tPresentation *p1, *p2;
        
    // Check base cases:
    // 1) Both queues are empty
    if(presentationQueue_empty(*q1) && presentationQueue_empty(*q2)) {
        return true;
    }
    // 2) One queue is empty and the other not
    if(presentationQueue_empty(*q1) || presentationQueue_empty(*q2)) {
        return false;
    }
    // 3) The head elements of the queue are different
    // Take head elements
    p1 = presentationQueue_head(*q1);
    p2 = presentationQueue_head(*q2);
    
    // Check that elements are not NULL
    assert(p1 != NULL);
    assert(p2 != NULL);
    
    // compare the head elements
    if(!presentation_compare(*p1, *p2)) {                
        // The elements are different.
        return false;
    }    
    
    // Dequeue the head elements from queues
    presentationQueue_dequeue(q1);
    presentationQueue_dequeue(q2);
    
    // Recursion. Two queues are equals if the heads are equals and the queues without the head are equals. Compare the remaining queues once dequeued the head.        
    return presentationQueue_compareRecursive(q1, q2);
}


// Compare two presentation queues
bool presentationQueue_compare(tPresentationQueue q1, tPresentationQueue q2) {
    bool equals;
    tPresentationQueue q1_dup, q2_dup;
        
    // Make a copy of the queues to avoid modifications
    presentationQueue_duplicate(&q1_dup, q1);
    presentationQueue_duplicate(&q2_dup, q2);
    
    // Compare both queues
    //equals = presentationQueue_compareIterative(&q1_dup, &q2_dup);
    equals = presentationQueue_compareRecursive(&q1_dup, &q2_dup);

    // Remove the remaining elements
    presentationQueue_free(&q1_dup);
    presentationQueue_free(&q2_dup);
    
    return equals;
}


// Compare two organization scores
bool organizationScore_compare(tPresentation p1, tPresentation p2) {    
    // Compare the values of both structures
    return (organization_equals(&p1.organization, &p2.organization) && p1.score == p2.score);
}


// Duplicate an organization score
void organizationScore_duplicate(tPresentation* dst, tPresentation src) {  

    // Check preconditions
    assert(dst!=NULL);
  
    // Copy the values of both structures
    dst->organization = src.organization;
    dst->score = src.score;
}


// Initialize a presentation structure
void presentation_init(tPresentation *object, tOrganization *organization, double score, const char* presentationTitle, const char* presentationTopic) {
    // Check preconditions
    assert(object != NULL);
    assert(organization != NULL);
    assert(presentationTitle != NULL);
    assert(presentationTopic != NULL);
    
   // object->organization = organization;
    object->score = score;
    // Allocate the memory for organization name, using the length of the provided text plus 1 space for the "end of string" char '\0'. To allocate memory we use the malloc command.    
    object->presentationTitle = (char*) malloc((strlen(presentationTitle) + 1) * sizeof(char));
    object->presentationTopic = (char*) malloc((strlen(presentationTopic) + 1) * sizeof(char));
    strcpy(object->presentationTitle, presentationTitle);
    strcpy(object->presentationTopic, presentationTopic);
   
    organization_cpy(&object->organization, organization);
}


// Duplicate a presentation
void presentation_duplicate(tPresentation* dst, tPresentation src) {    
    
    // Check preconditions
    assert(dst!=NULL);
    
    
    // Allocate the memory for organization name, using the length of the provided text plus 1 space for the "end of string" char '\0'. To allocate memory we use the malloc command.    
    dst->presentationTitle = (char*) malloc((strlen(src.presentationTitle) + 1) * sizeof(char));
    dst->presentationTopic = (char*) malloc((strlen(src.presentationTopic) + 1) * sizeof(char));
    
    // Copy the values of both structures
    strcpy(dst->presentationTitle, src.presentationTitle);
    strcpy(dst->presentationTopic, src.presentationTopic);
    dst->score = src.score ;
    organization_cpy(&dst->organization, &src.organization);

    //organizationScore_duplicate(&(dst->score), src.score);
}


// Compare two presentations
bool presentation_compare(tPresentation p1, tPresentation p2) {    
    // Compare the values of both structures
    
    
    return (strcmp(p1.presentationTitle, p2.presentationTitle)==0 
    && strcmp(p1.presentationTopic, p2.presentationTopic)==0 
    && p1.score == p2.score && organization_equals(&p1.organization,&p2.organization));
}


// Create the presentation queue
void presentationQueue_createQueue(tPresentationQueue* queue) {
    // PR2 EX1
	queue->first=NULL;
	queue->last=NULL;
}


// Enqueue a new presentation to the presentation queue
tError presentationQueue_enqueue(tPresentationQueue* queue, tPresentation presentation) {
    // PR2 EX1
	tPresentationQueueNode *tmp;
	
		tmp = (tPresentationQueueNode*) malloc(sizeof(tPresentationQueueNode));
		if (tmp == NULL){
			return ERR_MEMORY_ERROR;
		}
		else {
			tmp->e = presentation;
			tmp->next = NULL;
			if (queue->first == NULL){
				queue->first = tmp;
			} else {
			queue->last->next = tmp;
			}
			queue->last = tmp;
		}
		return OK;
}


// Check if the queue is empty
bool presentationQueue_empty(tPresentationQueue queue) {
    // PR2 EX1
	return queue.first == NULL;
}


// Return the first element from the queue
tPresentation* presentationQueue_head(tPresentationQueue queue) {
    // PR2 EX2
	if (presentationQueue_empty(queue))
		return NULL;
	return &queue.first->e;
}


// Dequeue a presentation from the presentation queue
void presentationQueue_dequeue(tPresentationQueue* queue) {
    // PR2 EX2
	if (presentationQueue_empty(*queue) == false){
		tPresentationQueueNode *node;
		node=queue->first;
		queue->first=queue->first->next;
		if (queue->first == NULL)
			queue->last = NULL;
		//destroy(node->e);//may be necessary. Query posted in the forum
		free(node);
	}
		
}


// Remove all elements of the queue
void presentationQueue_free(tPresentationQueue* queue) {
    // PR2 EX2
	while (presentationQueue_empty(*queue) == false){
		presentationQueue_dequeue(queue);
	}
}


// Get the average score for an organization
double presentationQueue_getOrganizationScoreRecursive(tPresentationQueue *queue, tOrganization *organization) {    
    // PR2 EX3
    return ERR_NOT_IMPLEMENTED;
}


// Get a queue with all the presentations for an organization
tError presentationQueue_getOrganizationPresentationsRecursive(tPresentationQueue *queue, tOrganization *organization, tPresentationQueue* presentations) {
    // PR2 EX3
    return ERR_NOT_IMPLEMENTED;
}

