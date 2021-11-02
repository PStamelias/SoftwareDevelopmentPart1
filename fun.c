#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	entry* node=(entry *)e;
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
	(*ix)->Nodeptr=NULL;
	for(struct entry* node=el->first_node;node!=NULL;node=node->next){
		word* w=node->my_word;
		if((*ix)->Nodeptr==NULL){
			struct Node_Index* riza=malloc(sizeof(struct Node_Index));
			riza->child_counter=0;
			riza->Root_Node=NULL;
			riza->the_word=malloc((strlen(w)+1)*sizeof(char));
			strcpy(riza->the_word,w);
			(*ix)->Nodeptr=riza;
		}
		else{
			struct Node_Index* start=(*ix)->Nodeptr;
			while(1){
				int distance=0;
				if(val==1){
					distance=hamming_distance(start->the_word,w);
				}
				if(val==2){
					//distance=edit_distance(start->the_word,w);
				}
				if(start->child_counter==0){
					struct Node_Index* new_node=malloc(sizeof(struct Node_Index));
					new_node->child_counter=0;
					new_node->Root_Node=NULL;
					new_node->the_word=malloc((strlen(w)+1)*sizeof(char));
					strcpy(new_node->the_word,w);
					start->Root_Node=realloc(start->Root_Node,(start->child_counter+1)*sizeof(struct root));
					start->Root_Node[start->child_counter].Id_root=distance;
					start->Root_Node[start->child_counter].childptr=new_node;
					start->child_counter+=1;
				}
				else{
					int found=0;
					struct Node_Index* if_exists=NULL;
					for(int i=0;i<start->child_counter;i++){
						if(distance==start->Root_Node[i].Id_root){
							found=1;
							if_exists=start->Root_Node[i].childptr;
							break;
						}
					}
					if(found==1){
						start=if_exists;
						continue;
					}
					else{
						struct Node_Index* new_node=malloc(sizeof(struct Node_Index));
						new_node->child_counter=0;
						new_node->Root_Node=NULL;
						new_node->the_word=malloc((strlen(w)+1)*sizeof(char));
						strcpy(new_node->the_word,w);
						start->Root_Node=realloc(start->Root_Node,(start->child_counter+1)*sizeof(struct root));
						start->Root_Node[start->child_counter].Id_root=distance;
						start->Root_Node[start->child_counter].childptr=new_node;
						start->child_counter+=1;
					}
				}
			}
		}
	}
	return EC_SUCCESS;
}
enum error_code lookup_entry_index(const word* w,Index* ix,int threshold,entry_list** result){
	return EC_SUCCESS;
}
enum error_code destroy_entry_index(Index* ix){
	if(ix==NULL)
		return EC_FAIL;
	struct Node_Index* riza_node=(*ix).Nodeptr;
	Destroy_Index_Node(riza_node);
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
struct Info_Table* deduplication_method(char* filename){
	FILE* f=fopen(filename,"r");
	if(f==NULL)
		return NULL;
	char word[MAX_LENGTH_WORD];
	struct Info_Table* the_node=malloc(sizeof(struct Info_Table));
	the_node->counter=0;
	the_node->nodes=NULL;
	the_node->nodes=realloc(the_node->nodes,(the_node->counter+1)*sizeof(struct Name_info));
	the_node->nodes[the_node->counter].counter=0;
	the_node->nodes[the_node->counter].names=NULL;
	the_node->nodes[the_node->counter].names=realloc(the_node->nodes[the_node->counter].names,(the_node->nodes[the_node->counter].counter+1)*sizeof(char* ));
	the_node->counter+=1;
	char c;
	int k=0;
	int found=0;
	while ((c = fgetc(f)) != EOF){
		if(c=='&'){
			the_node->nodes=realloc(the_node->nodes,(the_node->counter+1)*sizeof(struct Name_info));
			the_node->nodes[the_node->counter].counter=0;
			the_node->nodes[the_node->counter].names=NULL;
			the_node->nodes[the_node->counter].names=realloc(the_node->nodes[the_node->counter].names,(the_node->nodes[the_node->counter].counter+1)*sizeof(char* ));
			the_node->counter+=1;
			memset(word,0,MAX_LENGTH_WORD);
        	k=0;
			continue;
        }
        if(c==' '||c=='\n'){
        	found=0;
        	word[k]='\0';
        	for(int i=0;i<the_node->nodes[the_node->counter-1].counter;i++){
        		if(!strcmp(word,the_node->nodes[the_node->counter-1].names[i]))
        			found=1;
        	}
        	if(found==0){
        		the_node->nodes[the_node->counter-1].names=realloc(the_node->nodes[the_node->counter-1].names,(the_node->nodes->counter+1)*sizeof(char* ));
        		the_node->nodes[the_node->counter-1].names[the_node->nodes[the_node->counter-1].counter]=NULL;
        		the_node->nodes[the_node->counter-1].names[the_node->nodes[the_node->counter-1].counter]=(char* )malloc((k+1)*sizeof(char));
        		strcpy(the_node->nodes[the_node->counter-1].names[the_node->nodes[the_node->counter-1].counter++],word);
        	}
        	memset(word,0,MAX_LENGTH_WORD);
        	k=0;
        	continue;
        }
        word[k++]=c;
	}
	fclose(f);
	return the_node;
}
void delete_name_info(struct Info_Table* node){
	for(int j=0;j<node->counter;j++){
		for(int k=0;k<node->nodes[j].counter;k++)
			free(node->nodes[j].names[k]);
		free(node->nodes[j].names);
	}
	free(node->nodes);
	free(node);
}
void Destroy_Index_Node(struct Node_Index* node){
	for(int i=0;i<node->child_counter;i++){
		struct Node_Index* komvos=node->Root_Node[i].childptr;
		Destroy_Index_Node(komvos);
	}
	free(node->the_word);
	free(node);
}