#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "guest.h"

// Initialize the guest structure
tError guest_init(tGuest* object, const char* username, const char* name, const char* mail) {

    // Verify pre conditions
    assert(object != NULL);
    assert(username != NULL);
    assert(name != NULL);
    assert(mail != NULL);
    
    // Check that the mail have the correct format
    if(!check_mail(mail)) {
        return ERR_INVALID;
    }
    
    // Allocate the memory for all the fields, using the length of the provided text plus 1 space for the "end of string" char '\0'. To allocate memory we use the malloc command.
    object->username = (char*) malloc((strlen(username) + 1) * sizeof(char));
    object->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
    object->mail = (char*) malloc((strlen(mail) + 1) * sizeof(char));
    
    // Check that memory has been allocated for all fields. Pointer must be different from NULL.
    if(object->username == NULL || object->name == NULL || object->mail == NULL) {
        // Some of the fields have a NULL value, what means that we found some problem allocating the memory
        return ERR_MEMORY_ERROR;
    }
    
    // Once the memory is allocated, copy the data. As the fields are strings, we need to use the string copy function strcpy. 
    strcpy(object->username, username);
    strcpy(object->name, name);
    strcpy(object->mail, mail);
    
    return OK;    
}

// Remove the memory used by guest structure
void guest_free(tGuest* object) {
    
    // Verify pre conditions
    assert(object != NULL);
    
    // All memory allocated with malloc and realloc needs to be freed using the free command. In this case, as we use malloc to allocate the fields, we have to free them
    if(object->username != NULL) {
        free(object->username);
        object->username = NULL;
    }
    if(object->name != NULL) {
        free(object->name);
        object->name = NULL;
    }
    if(object->mail != NULL) {
        free(object->mail);
        object->mail = NULL;
    }
}

// Compare two guests
bool guest_equals(tGuest* guest1, tGuest* guest2) {
    
    // Verify pre conditions
    assert(guest1 != NULL);
    assert(guest2 != NULL);
    
    // To see if two guests are equals, we need to see ALL the values for their fields are equals.    
    // Strings are pointers to a table of chars, therefore, cannot be compared  as  " guest1->username == guest2->username ". We need to use a string comparison function    
    
    if(strcmp(guest1->username, guest2->username) != 0) {
        // Usernames are different
        return false;
    }
    
    if(strcmp(guest1->name, guest2->name) != 0) {
        // Names are different
        return false;
    }
    
    if(strcmp(guest1->mail, guest2->mail) != 0) {
        // Mails are different
        return false;
    }
    
    // All fields have the same value
    return true;
}

// Compare two Table of guests. There are equals if they have the same number of guest, they have the same username. The order of guests of two tables could be different
bool guestTable_equals(tGuestTable* guestTable1, tGuestTable* guestTable2){
    // Verify pre conditions
    assert(guestTable1 != NULL);
    assert(guestTable2 != NULL);

    int i;
    if(guestTable1->size != guestTable2->size){
      return false;
    }

    for(i=0; i< guestTable1->size;i++)
    {
        // Uses "find" because the order of guests could be different
        if(!guestTable_find(guestTable1, guestTable2->elements[i].username)) {
            // Usernames are different
            return false;
        }
    }
    
    return true;
}

// Copy the data of a guest to another guest
tError guest_cpy(tGuest* dst, tGuest* src) {
    
    // Verify pre conditions
    assert(dst != NULL);
    assert(src != NULL);
    
    // Free the space used by destination object. An initialized object is assumed.
    guest_free(dst);
    
    // Initialize the element with the new data
    guest_init(dst, src->username, src->name, src->mail);
    
    return OK;
}

// Initialize the table of guests
void guestTable_init(tGuestTable* table) {
    // Verify pre conditions
    assert(table != NULL);    
    
    // The initialization of a table is to set it to the empty state. That is, with 0 elements. 
    table->size = 0;
    // Using dynamic memory, the pointer to the elements must be set to NULL (no memory allocated). This is the main difference with respect to the user of static memory, were data was allways initialized (tGuest elements[MAX_ELEMENTS])
    table->elements = NULL;
}

// Remove the memory used by guestTable structure
void guestTable_free(tGuestTable* object) {
    // Verify pre conditions
    assert(object != NULL);
    
    // All memory allocated with malloc and realloc needs to be freed using the free command. In this case, as we use malloc/realloc to allocate the elements, and need to free them.
    if(object->elements != NULL) {
        free(object->elements);
        object->elements = NULL;
    }
    // As the table is now empty, assign the size to 0.
    object->size = 0;
}

// Add a new guest to the table
tError guestTable_add(tGuestTable* table, tGuest* guest) {
    // Verify pre conditions
    assert(table != NULL);
    assert(guest != NULL);
    
    // Check if guests already is on the table
    if (guestTable_find(table, guest->username))
        return ERR_DUPLICATED;
        
    // The first step is to allocate the required space. There are two methods to manage the memory (malloc and realloc). Malloc allows to allocate a new memory block, while realloc allows to modify an existing memory block.    
    if(table->size == 0) {
        // Empty table
        
        // Increase the number of elements of the table
        table->size = 1;
        
        // Since the table is empty, and we do not have any previous memory block, we have to use malloc. The amount of memory we need is the number of elements (will be 1) times the size of one element, which is computed by sizeof(type). In this case the type is tGuest.
        table->elements = (tGuest*) malloc(table->size * sizeof(tGuest));        
    } else {
        // table with elements
        
        // Increase the number of elements of the table
        table->size = table->size + 1 ;
        
        // Since the table is not empty, we already have a memory block. We need to modify the size of this block, using the realloc command. The amount of memory we need is the number of elements times the size of one element, which is computed by sizeof(type). In this case the type is tGuest. We provide the previous block of memory.
        table->elements = (tGuest*) realloc(table->elements, table->size * sizeof(tGuest));         
    }
    
    // Check that the memory has been allocated
    if(table->elements == NULL) {
        // Error allocating or reallocating the memory
        return ERR_MEMORY_ERROR;
    }
    
    // Once we have the block of memory, which is an array of tGuest elements, we initialize the new element (which is the last one). The last element is " table->elements[table->size - 1] " (we start counting at 0)
    guest_init(&(table->elements[table->size - 1]), guest->username, guest->name, guest->mail);
    
    return OK;
}

// Remove a guest from the table
tError guestTable_remove(tGuestTable* table, tGuest* guest) {
    int i;
    bool found;
    
    // Verify pre conditions
    assert(table != NULL);
    assert(guest != NULL);
    
    // To remove an element of a table, we will move all elements after this element one position, to fill the space of the removed element.
    found = false;
    for(i=0; i<table->size; i++) {
        // If the element has been found. Displace this element to the previous element (will never happend for the first one). We use the ADDRESS of the previous element &(table->elements[i-1]) as destination, and ADDRESS of the current element &(table->elements[i]) as source.
        if(found) {
            // Check the return code to detect memory allocation errors
            if(guest_cpy(&(table->elements[i-1]), &(table->elements[i])) == ERR_MEMORY_ERROR) {
                // Error allocating memory. Just stop the process and return memory error.
                return ERR_MEMORY_ERROR;
            }
        } else if(strcmp(table->elements[i].username, guest->username) == 0) {
            // The current element is the element we want to remove. Set found flag to true to start element movement.
            found = true;
        }
    }    
    
    // Once removed the element, we need to modify the memory used by the table.
    if(found) {
        // Modify the number of elements
        table->size = table->size - 1;
        
        // If we are removing the last element, we will assign the pointer to NULL, since we cannot allocate zero bytes
        if (table->size == 0) {
            table->elements = NULL;
        } else {                
            // Modify the used memory. As we are modifying a previously allocated block, we need to use the realloc command.
            table->elements = (tGuest*) realloc(table->elements, table->size * sizeof(tGuest));
            
            // Check that the memory has been allocated
            if(table->elements == NULL) {
                // Error allocating or reallocating the memory
                return ERR_MEMORY_ERROR;
            }        
        }
    } else {
        // If the element was not in the table, return an error.
        return ERR_NOT_FOUND;
    }         
    
    return OK;
}

// Get guest by username
tGuest* guestTable_find(tGuestTable* table, const char* username) {
    int i;
    
    // Verify pre conditions
    assert(table != NULL);
    assert(username != NULL);
    
    // Search over the table and return once we found the element.
    for(i=0; i<table->size; i++) {
        if(strcmp(table->elements[i].username, username) == 0) {
            // We return the ADDRESS (&) of the element, which is a pointer to the element
            return &(table->elements[i]);
        }
    }
    
    // The element has not been found. Return NULL (empty pointer).
    return NULL;
}

// Get the size of a the table
unsigned int guestTable_size(tGuestTable* table) {
    // Verify pre conditions
    assert(table != NULL);
    
    // The size of the table is the number of elements. This value is stored in the "size" field.
    return table->size;
}

// Check the format of an email
bool check_mail(const char* mail) {
    // PR1 EX4
    // return true;
    char *pAt = NULL;
    
    // Verify pre conditions
    assert(mail != NULL);
    
    // The format will be <string>@<string>.<string>
    
    // Check if the mail have the char '@'
    pAt = strchr(mail, '@');
    if( pAt == NULL) {
        return false;
    }
    
    // Check if the mail have the char '.' after '@'
    if(strchr(pAt, '.') == NULL) {
        return false;
    }
    
    // Check that the mail do not ends with a '.'
    if(mail[strlen(mail) - 1] == '.') {
        return false;
    }    
        
    return true;
}
