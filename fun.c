#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LENGTH_WORD 31
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
	e->my_word=NULL;
	free(e);
	e=NULL;
	if(e!=NULL)
		return EC_FAIL;
	return EC_SUCCESS;
}
enum error_code create_entry_list(entry_list** el){
	if(*el!=NULL)
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
	entry* node=(entry *)e;
	if(el==NULL)
		return EC_FAIL;
	if(el->first_node==NULL){
		el->first_node=node;
		el->counter=1;
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
	el->counter++;
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
	el=NULL;
	if(el!=NULL)
		return EC_FAIL;
	return EC_SUCCESS;
}
enum error_code build_entry_index(const entry_list* el,MatchType type,Index** ix){
	int val=0;
	if(type==1)//MT_HAMMING_DIST 
		val=1;
	if(type==2)//MT_EDIT_DIST
		val=2;
	if(el==NULL)
		return EC_FAIL;
	if(*ix!=NULL)
		return EC_FAIL;
	*ix=malloc(sizeof(Index));
	(*ix)->root=NULL;
	if(val==1)
		(*ix)->type=0;
	if(val==2)
		(*ix)->type=1;
	for(struct entry* node=el->first_node;node!=NULL;node=node->next){
		word* curr_word=node->my_word;
		if((*ix)->root==NULL){
			struct NodeIndex* e=malloc(sizeof(struct NodeIndex));
			e->next=NULL;
			e->firstChild=NULL;
			e->distance=0;
			e->wd=NULL;
			e->wd=malloc((strlen(curr_word)+1)*sizeof(char));
			strcpy(e->wd,curr_word);
			(*ix)->root=e;
		}
		else{
			struct NodeIndex* start=(*ix)->root;
			while(1){
				int distance=0;
				if(val==1){
					distance=hamming_distance(start->wd,curr_word);
				}
				if(val==2){
					distance=edit_distance(start->wd,curr_word,0);
				}
				int found=0;
				struct NodeIndex* target=NULL;
				for(struct NodeIndex* c=start->firstChild;c!=NULL;c=c->next){
					if(distance==c->distance){
						found=1;
						target=c;
						break;
					}
				}
				if(found==1){
					start=target;
					continue;
				}
				struct NodeIndex* new_some=malloc(sizeof(struct NodeIndex));
				new_some->next=NULL;
				new_some->firstChild=NULL;
				new_some->distance=distance;
				new_some->wd=NULL;
				new_some->wd=malloc((strlen(curr_word)+1)*sizeof(char));
				strcpy(new_some->wd,curr_word);
				struct NodeIndex* c=start->firstChild;
				if(c==NULL){
					start->firstChild=new_some;
				}
				else{
					while(1){
						if(c->next==NULL){
							c->next=new_some;
							break;
						}
						c=c->next;
					}	
				}
				break;
			}
		}
	}
	return EC_SUCCESS;
}

enum error_code lookup_entry_index(word* w,Index* ix,int threshold,entry_list** result){
    if(ix==NULL){
		return EC_FAIL;
    }
    if(ix->root==NULL){
        return EC_FAIL;
    }
    int d, cd;
    int bot, ceil;
    struct NodeIndex* curr;
    entry* e = NULL;
    bool type = ix->type;//what method do we use for distance
    struct StackNode* candidate_list = NULL;
    push_stack(&candidate_list, &(ix->root));
    struct NodeIndex* children = NULL;
    while(candidate_list!=NULL){
        curr = pop_stack(&candidate_list);
        if(type){
            d = edit_distance(curr->wd, w, 0);
        }else{
            d = hamming_distance(curr->wd, w);
        }
        if(d <= threshold){
            bot = threshold - d;
            ceil = threshold + d;
            create_entry(curr->wd, &e);
            add_entry(*result, e);
            e = NULL;
        }else{
            bot = d - threshold;
            ceil = threshold + d;
        }
        children = curr->firstChild;
        while(children != NULL){
            if(children->distance >= bot && children->distance <= ceil){
                push_stack(&candidate_list, &children);
            }
            children = children->next;
        }
    }
    
	if(*result!=NULL){
		return EC_FAIL;
    }
	return EC_SUCCESS;
}

void push_stack(struct StackNode** list, struct NodeIndex** n){
    struct StackNode* temp = malloc(sizeof(struct StackNode));
    temp->node = *n;
    temp->next = *list;
    *list = temp;
}
struct NodeIndex* pop_stack(struct StackNode** list){
    struct StackNode* temp = *list;
    struct NodeIndex* ret = (*list)->node;
    *list = (*list)->next;
    free(temp);
    return ret;
}

enum error_code destroy_entry_index(Index* ix){
	if(ix==NULL)
		return EC_FAIL;
	destroy_index_nodes(ix->root);
	free(ix);
	ix=NULL;
	if(ix!=NULL)
		return EC_FAIL;	
	return EC_SUCCESS;
}

int edit_distance(char* word1, char* word2, int sp){ //sp is the starting position of the longest word
    int distance = 0;
    int maxlen, minlen, i, new_distance;
    if(strlen(word1) < strlen(word2)){
        maxlen = strlen(word2) - sp;
        minlen = strlen(word1);
    }else if(strlen(word1) >= strlen(word2)){
        maxlen = strlen(word1) - sp;
        minlen = strlen(word2);
    }
    //compare the letters of the same position
    //until you reach the end of at least one word
    for(i=0; i<minlen; i++){
        if(strlen(word1) < strlen(word2)){
            if(word1[i] != word2[sp+i]){
                distance++;
            }
        }else{
            if(word1[sp+i] != word2[i]){
                distance++;
            }
        }
    }
    distance = distance + (maxlen - i);     //add the remaining length to the distance
    if(maxlen == minlen){
        return distance;
    }else{
        new_distance = 1 + edit_distance(word1, word2, sp+1);  // cut out the first letter
        if(new_distance < distance){
            return new_distance;
        }else{
            return distance;
        }
    }
}

int hamming_distance(char* word1, char* word2){
    int distance = 0;
    int maxlen, minlen, i;
    if(strlen(word1) < strlen(word2)){
        maxlen = strlen(word2);
        minlen = strlen(word1);
    }else if(strlen(word1) >= strlen(word2)){
        maxlen = strlen(word1);
        minlen = strlen(word2);
    }
    //compare the letters of the same position
    //until you reach the end of at least one word
    for(i=0; i<minlen; i++){
        if(word1[i] != word2[i]){
            distance++;
        }
    }
    distance = distance + (maxlen - i);     //add the remaining length to the distance
    return distance;
}



void destroy_index_nodes(struct NodeIndex* node){
	for(struct NodeIndex* s=node->firstChild;s!=NULL;s=s->next){
		destroy_index_nodes(s);
	}
	free(node->wd);
	free(node);
}
struct Name_Info* deduplication_method(char* filename){
	FILE* f=fopen(filename,"r");
	if(f==NULL)
		return NULL;
	char c;
	int coun=0;
	while ((c = fgetc(f)) != EOF){
        if(c=='&')
        	coun+=1;
    }
    struct Name_Info* the_node=malloc(sizeof(struct Name_Info));
    rewind(f);
    the_node->counter=coun+1;
    the_node->ptr=malloc(the_node->counter*sizeof(struct Name*));
    for(int i=0;i<the_node->counter;i++)
    	the_node->ptr[i]=NULL;
    int pos=0;
    char word[MAX_LENGTH_WORD];
    int k=0;
    while ((c = fgetc(f)) != EOF){
    	if(c==' '||c=='\n'){
    		word[k]='\0';
    		if(!strcmp(word,"\0"))
    			continue;
    		if(the_node->ptr[pos]==NULL){
    			struct Name* node=malloc(sizeof(struct Name));
    			node->next=NULL;
    			node->the_name=malloc((strlen(word)+1)*sizeof(char));
    			strcpy(node->the_name,word);
    			the_node->ptr[pos]=node;
    		}
    		else{
    			int found=0;
    			for(struct Name* start=the_node->ptr[pos];start!=NULL;start=start->next){
    				if(!strcmp(start->the_name,word)){
    					found=1;
    					break;
    				}
    			}
    			if(found==0){
    				struct Name* node=malloc(sizeof(struct Name));
	    			node->next=NULL;
	    			node->the_name=malloc((strlen(word)+1)*sizeof(char));
	    			strcpy(node->the_name,word);
	    			struct Name* s=the_node->ptr[pos];
	    			while(1){
	    				if(s->next==NULL){
	    					s->next=node;
	    					break;
	    				}
	    				s=s->next;
	    			}
    			}
    		}
    		memset(word,0,MAX_LENGTH_WORD);
    		k=0;
    		continue;
    	}
    	if(c=='&'){
    		pos++;
    		continue;
    	}
    	word[k++]=c;
    }
	fclose(f);
	return the_node;
}
enum error_code delete_name_info(struct Name_Info* node){
	for(int i=0;i<node->counter;i++){
		struct Name* e=node->ptr[i];
		struct Name* NexName=e->next;
		while(1){
			free(e->the_name);
			free(e);
			if(NexName==NULL)
				break;
			e=NexName;
			NexName=NexName->next;
		}
	}
	free(node->ptr);
	free(node);
	node=NULL;
	if(node!=NULL)
		return EC_FAIL;
	return EC_SUCCESS;
}