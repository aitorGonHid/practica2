#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "congress.h"
#include <string.h>
#include "organization.h"
#include "presentation.h"

// Initialize the congress
void congress_init(tCongress* object, char* name) {
    // PR1 EX4
    
    // Verify pre conditions
    assert(object != NULL);    
    assert(name != NULL);
    
    // Allocate the memory for congress name, using the length of the provided text plus 1 space for the "end of string" char '\0'. To allocate memory we use the malloc command.    
    object->name = (char*) malloc((strlen(name) + 1) * sizeof(char));   
    strcpy(object->name, name);  
   
    object->organizations = (tOrganizationTable*) malloc(sizeof(tOrganizationTable)) ;     
    // Initialize organizations
    organizationTable_init(object->organizations);  

    // PR2 EX1 
    // Initialize presentations queue
	presentationQueue_createQueue(&(object->presentations));
}

// Remove all data for a congress
void congress_free(tCongress* object) {
    // PR1 EX4
    assert(object!=NULL);
    // Remove data from organizations table
    organizationTable_free(object->organizations);
    
    // PR2 EX2    
    // Remove data from presentations queue
    presentationQueue_free(&object->presentations);
}

// Register a new organization
tError congress_registerOrganization(tCongress* object, tOrganization* organization) {

    // PR1 EX4    
    assert(object != NULL);   
    assert(organization != NULL);   
    
    tError err;
    
    // Check if the organization already exists
    int i;
    for(i=0; i < object->organizations->size;i++)
    {   
        if(organizationTable_find(object->organizations, organization->name) != NULL) {
            return ERR_DUPLICATED;
        }
    }
    
    // Add the organization to the table
    err = organizationTable_add(object->organizations, organization);
    
    return err;    
}

// Remove a organization
tError congress_removeOrganization(tCongress* object, tOrganization* organization) {
    // PR1 EX4
    assert(object != NULL); 
    assert(organization != NULL); 
    
    // Get the organization with the organizationName
    organization = organizationTable_find(object->organizations, organization->name);
    
    if(organization == NULL) {
        return ERR_NOT_FOUND;
    }
    
    return organizationTable_remove(object->organizations, organization);
}

// Find a organization
tOrganization* congress_findOrganization(tCongress* object, const char* organizationName) {
    // PR1 EX4   
    assert(object != NULL); 
    assert(organizationName != NULL);
 
    return organizationTable_find(object->organizations, organizationName);
}


tError congress_getGuests(tCongress* object, tGuestTable* guests) {
    // PR1 EX4

    assert(object != NULL);
    assert(guests != NULL);    

    int i, j;
    tError error;
    error = OK;    

    for (i=0; i<object->organizations->size; i++) {
        for (j=0; j<object->organizations->elements[i].guests->size; j++) {
            error = guestTable_add(guests, &(object->organizations->elements[i].guests->elements[j]));
        }
    }   
    return error;
}


// Add a new presentation
tError congress_addPresentation(tCongress* object, const char* organization_name, double score, const char* presentationTitle, const char* presentationTopic) {
    // PR2 EX1
	tPresentation pres;
	tOrganization *org;
	//find the organization passed as parameter into the congress
	org = organizationTable_find(object->organizations, organization_name);
	if (org == NULL)
		return ERR_INVALID_ORGANIZATION;//organization doesn't exist
	//initialize presentatión. Giving space in memory and addind the provided data by the function calling
	presentation_init(&pres,org,score,presentationTitle,presentationTopic);
	//Add the new presentation to the queue. 
	//The presentationQueue_enqueue function provides the error outputs (OK/ERR_MEMORY_ERROR) that verify the normal functioning
	presentationQueue_enqueue(&object->presentations,pres);
}


// Get if an organization wins on a topic. Wins means best score on a topic
int congress_getOrganizationWins(tCongress* object, const char* organization_name, const char* topic) {
    // PR2 EX2
    return ERR_NOT_IMPLEMENTED;
}


// Get the average score for an organization
double congress_getOrganizationScore(tCongress* object, const char* organization_name) {
    // PR2 EX3   
    return ERR_NOT_IMPLEMENTED;
}


// Get a queue with all the presentations for an organization
tError congress_getOrganizationPresentations(tCongress* object, const char* organization_name, tPresentationQueue* presentations) {
    // PR2 EX3   
    return ERR_NOT_IMPLEMENTED;
}