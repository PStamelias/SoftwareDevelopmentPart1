#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
enum error_code create_entry(const word* w,entry** e){
	if(*e!=NULL)
		return EC_FAIL;
	if(strlen(w) == 0)
		return EC_FAIL;
	*e=malloc(sizeof(entry));
	(*e)->next=NULL;
	(*e)->my_word=malloc((strlen(w)+1)*sizeof(char));
	(*e)->payload=NULL;
	strcpy((*e)->my_word,w);
	return EC_SUCCESS;
}
enum error_code destroy_entry(entry* e){
	if(e==NULL)
		return EC_FAIL;
	free(e->my_word);
	free(e);
	return EC_SUCCESS;
}
enum error_code create_entry_list(entry_list** el){
	if(*el!=NULL)
		return EC_FAIL;
	if((*el)->first_node!=NULL||(*el)->current_node!=NULL||(*el)->counter!=0)
		return EC_FAIL;
	entry_list* node=malloc(sizeof(entry_list));
	node->first_node=NULL;
	node->current_node=NULL;
	node->counter=0;
	*el=node;
	return EC_SUCCESS;

}
unsigned int get_number_entries(const entry_list* el){
	unsigned int coun=0;
	if(el==NULL)
		return coun;
	else
		return el->counter;
}
enum error_code add_entry(entry_list* el,const entry* e){
	if(e==NULL)
		return EC_FAIL;
	entry* node=e;
	if(el==NULL)
		return EC_FAIL;
	if(el->first_node==NULL){
		el->first_node=node;
		el->current_node=node;
		return EC_SUCCESS;
	}
	entry* start=el->first_node;
	while(1){
		if(start->next==NULL){
			start->next=node;
			el->current_node=node;
			break;
		}
		start=start->next;
	}
	return EC_SUCCESS;
}
entry* get_first(const entry_list* el){
	if(el==NULL)
		return NULL;
	if(el->counter==0||el->first_node==NULL||el->current_node==NULL)
		return NULL;
	entry* e=el->first_node;
	return e;
}
entry* get_next(const entry_list* el,const entry* e){
	if(el==NULL)
		return NULL;
	if(e==NULL)
		return NULL;
	return e->next;
}
enum error_code destroy_entry_list(entry_list* el){
	if(el==NULL)
		return EC_FAIL;
	if(el->counter==0){
		free(el);
		return EC_SUCCESS;
	}
	entry* start=el->first_node;
	entry* next_node=start->next;
	while(1){
		error_code e=destroy_entry(start);
		if(e==EC_FAIL)
			return EC_FAIL;
		start=next_node;
		if(start==NULL)
			break;
		next_node=next_node->next;
	}
	free(el);
	return EC_SUCCESS;
}
/*enum error_code build_entry_index(const entry_list* el,enum match_type,index* ix){

}
enum error_Code lookup_entry_index(const word* w,index* ix,int threshold,entry_list* result){

}
enum error_code destroy_entry_index(index* ix){

}*/