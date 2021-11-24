//Eric Wagner ew046 116484695
#include <string>
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <strings.h>
#include <cstring>
#include <math.h>
using namespace std;

#define MAX_STRING_LENG 256
#define MAX_NAME_LEN 6
#define NDIR_1D 2
#define NDIR_2D 4
typedef enum{X,Y} axis;
typedef enum{NW,NE,SW,SE} quadrant;
typedef enum{LEFT,RIGHT,BOTH} direction;
typedef char tName[MAX_NAME_LEN + 1];


struct Rectangle {
  tName Name;
  struct Rectangle *binSon[NDIR_1D];
  int Center[NDIR_1D];
  int Length[NDIR_1D];
  int Label;
};

struct bNode{
    struct bNode *binSon[NDIR_1D];
    struct Rectangle *rect;

};

struct colNode{
    struct colNode *binSon[NDIR_1D];
    struct Rectangle *rect;
};

struct cNode{
    struct cNode *spcSon[NDIR_2D];
    struct bNode *binSon[NDIR_1D];
};

struct mxCif{
    struct cNode *mxCifRoot;
    struct Rectangle World;
    int Id;
    
};

struct checkBoard{
    struct checkBoard *next;
    int x;
    int y;
    tName Name;
    struct checkBoard *prev;
};

struct mxCif mxCifTree;
struct Rectangle *recTree;
struct colNode *colHead = nullptr;
int rectangle_count;
int wl = 0;
struct checkBoard *chead = nullptr;


void initMxCifTree(void){
    mxCifTree.mxCifRoot = NULL;
    strcpy(mxCifTree.World.Name, "MX-CIF");
    //mxCifTree.World.Name = "MX-CIF";
    mxCifTree.World.binSon[LEFT] = NULL;
    mxCifTree.World.binSon[RIGHT] = NULL;
}

void initRectTree(void){
    recTree = NULL;
}

void INIT_QUADTREE(int w){
    initMxCifTree();
    int l = 2;
    int i =1;
    while (i < w){
        l = l*2;
        i++;
    }
    wl = l;
    mxCifTree.World.Length[0] = (l); 
    mxCifTree.World.Length[1] = (l); 
    mxCifTree.World.Center[0] = (l)/2;
    mxCifTree.World.Center[1] = (l)/2; 

    recTree = new Rectangle;
    recTree->Length[0] = l;
    recTree->Length[1] = l;
    recTree->Center[0] = l/2;
    recTree->Center[1] = l/2; 

    cout<<"initialized a quadtree of width ";
    cout<<(l);
    mxCifTree.mxCifRoot = new cNode;
}

void DISPLAY(){

}


void insert_to_collection(Rectangle * re, colNode** cn){
    //add to collection
    if (*cn == nullptr){
        colNode* r = new colNode;
        r->rect = new Rectangle;
        r->rect = re;
        *cn = r;

        *cn = new colNode;
        (*cn)->rect = new Rectangle;

        //(*cn)->rect->Name = re->Name;
        for(int i =0; i< 7;i++){
           (*cn)->rect->Name[i] = re->Name[i]; 
        } 
        (*cn)->rect->Center[0] = re->Center[0];
        (*cn)->rect->Center[1] = re->Center[1];
        (*cn)->rect->Length[0] = re->Length[0];
        (*cn)->rect->Length[1] = re->Length[1];
    } else{
        for (int i=0; i<7; i++){
            if (re->Name[i] > (*cn)->rect->Name[i]){
                insert_to_collection(re,&((*cn)->binSon[1]));
                return;
            } else if (re->Name[i] < (*cn)->rect->Name[i]) {
                insert_to_collection(re,&((*cn)->binSon[0]));
                return;
            }
        }
    }
}

Rectangle * find_in_collection(string n, colNode ** cn){
    Rectangle * r = new Rectangle;
    if ((*cn) == nullptr){
        return nullptr;
    }
    if ((*cn)->rect == nullptr){
        return nullptr;
    }
    if ((*cn)->rect->Name == nullptr){
        return  nullptr;
    }
    bool s = true;
    for (int i =0; i < n.length(); i++){
        if ((*cn)->rect->Name[i] != n.at(i)){
            s = false;
        }
    }
    if ((*cn)->rect->Name[0] != n.at(0)){
        s = false;
    }
    if (s == true){
        return ((*cn)->rect);
    }
    if ((*cn)->rect->Name == n){
        return (*cn)->rect;
    }
    
    for (int i=0; i<n.length(); i++){
        if ((*cn)->rect->Name[i] > n.at(i)){
           return find_in_collection(n, &((*cn)->binSon[0]));
        } else if ((*cn)->rect->Name[i] < n.at(i)){
           return find_in_collection(n, &((*cn)->binSon[1]));
        } 
    } 
    cout<<4;
    return nullptr; 
} 

void REC_LIST_RECTANGLES(colNode * collect){
    if (collect != NULL){
        REC_LIST_RECTANGLES(collect->binSon[0]);
        cout<<"\n";
        for (int i=0; i<7; i++){
            if (collect->rect->Name[i] != ' '){
                cout<<collect->rect->Name[i];
            }
        }
        cout<<" ";
        cout<<collect->rect->Center[0];
        cout<<" ";
        cout<<collect->rect->Center[1];
        cout<<" ";
        cout<<collect->rect->Length[0];
        cout<<" ";
        cout<<collect->rect->Length[1];
        cout<<" "; 
        REC_LIST_RECTANGLES(collect->binSon[1]);
    }
}

void LIST_RECTANGLES(){
    cout<<"listing ";
    cout<< rectangle_count;
    cout<<" rectangles: ";
    REC_LIST_RECTANGLES(colHead);
}

void CREATE_RECTANGLE(string R, int xc, int yc, int w, int h){
     Rectangle * rec = new Rectangle;
    for(int x=0; x< 7;x++){
        if (x < R.length()){
            rec->Name[x] = R.at(x);
        } else {
            rec->Name[x] = ' ';
        }
    }
    rec->Length[0] = w;
    rec->Length[1] = h;
    rec->Center[0] = xc;
    rec->Center[1] = yc;
    cout<<"created rectangle ";
    for(int x =0; x< R.length();x++){
        cout<<rec->Name[x];
    }
    //cout<<rec.Name;
    insert_to_collection(rec, &colHead);
    Rectangle * r = find_in_collection(rec->Name,&colHead);
    if (r != nullptr){
    //cout<<r->Name; 
    rectangle_count++;
    }
}

void CLIST_ADD(int x, int y, tName n){
    checkBoard * ce = new checkBoard;
    ce->x = x;
    ce->y = y;
    for(int i =0; i< 7;i++){
        ce->Name[i] = n[i]; 
    }
    ce->next = nullptr;
    if (chead == nullptr){
        chead = ce;
    } else{
        checkBoard* temp = chead;
        while(temp->next != nullptr){
            temp=temp->next;
        }
        ce->prev = temp;
        temp->next = ce;
    }
    
}

bool CLIST_CONTAINS(int x, int y){
    if (chead == nullptr){
        return false;
    } else{
        checkBoard* temp = chead;
        while(temp->next != nullptr){
            if (temp->x == x && temp->y == y){
                //cout<<"failed: intersects with ";
                cout<<temp->Name;
                return true;
            }
            temp=temp->next;
        }
        //return false;
    }
    return false;
}

void AXIS_INSERT(bNode ** bt, Rectangle * r, int cent, int len, int axis){
        if (cent > (r->Center[axis]+r->Length[axis])){
            //go left
            if ((*bt)->binSon[0] == nullptr){
                (*bt)->binSon[0] = new bNode;
                (*bt)->binSon[0]->binSon[0] = nullptr;
                (*bt)->binSon[0]->binSon[1] = nullptr;
                (*bt)->binSon[0]->rect = nullptr;
            }
            AXIS_INSERT(&((*bt)->binSon[0]),r,(cent - (len/4)),(len/2),axis); 
        } else if (cent < (r->Center[axis]-r->Length[axis])){
            //go right
            if ((*bt)->binSon[1] == nullptr){
                (*bt)->binSon[1] = new bNode;
                (*bt)->binSon[1]->binSon[0] = nullptr;
                (*bt)->binSon[1]->binSon[1] = nullptr;
                (*bt)->binSon[1]->rect = nullptr;
            } 
            AXIS_INSERT(&((*bt)->binSon[1]),r,(cent + (len/4)),(len/2),axis); 
        } else{
            //store in baxis tree
            if ((*bt)->rect->Name == nullptr){
                for (int x = (r->Center[0] - (r->Length[0])+1);x < (r->Center[0] + (r->Length[0])); x++){
                    for (int y = (r->Center[1] - (r->Length[1])+1);y < (r->Center[1] + (r->Length[1])); y++){
                        if (CLIST_CONTAINS(x,y) == true){
                            return;
                        }
                    }
                }
                Rectangle * re = new  Rectangle;
                re->Center[0] = r->Center[0];
                re->Center[1] = r->Center[1];
                re->Length[0] = r->Length[0];
                re->Length[1] = r->Length[1];
                for(int i =0; i< 7;i++){
                    re->Name[i] = r->Name[i];
                } 
                (*bt)->rect = re;
                cout<<"inserted rectangle ";
                cout<<(*bt)->rect->Name;
                for (int x = (r->Center[0] - (r->Length[0]));x <= (r->Center[0] + (r->Length[0])); x++){
                    for (int y = (r->Center[1] - (r->Length[1]));y <= (r->Center[1] + (r->Length[1])); y++){
                        CLIST_ADD(x,y,r->Name);
                }
            }
        }
 } 
}

void REC_INSERT(Rectangle * r, cNode ** qt, Rectangle  * zworld){
    if (qt == nullptr){
        return;
    }
    if (((zworld->Center[0]) <= (r->Center[0] + r->Length[0])) &&
       (zworld->Center[0] >= (r->Center[0] - r->Length[0])) &&
       (zworld->Center[1] <= (r->Center[1] + r->Length[1])) &&
       (zworld->Center[1] >= (r->Center[1] - r->Length[1])))
       {
           //origin
           (*qt)->binSon[1] = new bNode;
           (*qt)->binSon[1]->rect = r;
           (*qt)->binSon[1]->binSon[0] = nullptr;
           (*qt)->binSon[1]->binSon[1] = nullptr;
            for (int x = (r->Center[0] - (r->Length[0]));x < (r->Center[0] + (r->Length[0])); x++){
                for (int y = (r->Center[1] - (r->Length[1]));y < (r->Center[1] + (r->Length[1])); y++){
                    if (CLIST_CONTAINS(x,y) == true){
                            cout<<"overlap";
                            return;
                        }
                }
            }
            for (int x = (r->Center[0] - (r->Length[0]));x <= (r->Center[0] + (r->Length[0])); x++){
                for (int y = (r->Center[1] - (r->Length[1]));y <= (r->Center[1] + (r->Length[1])); y++){
                    CLIST_ADD(x,y,r->Name);
                }
            }
           cout<<"inserted origin rectangle ";
           cout<<r->Name;
           return;
       } else if (
           (zworld->Center[0] <= (r->Center[0] + r->Length[0])) &&
           (zworld->Center[0] >= (r->Center[0] - r->Length[0]))){
               //itersects y axis (call seprate function)
               (*qt)->binSon[1] = new bNode;
               (*qt)->binSon[1]->binSon[0] = nullptr; 
               (*qt)->binSon[1]->binSon[1] = nullptr;
               (*qt)->binSon[1]->rect = nullptr;
               AXIS_INSERT(&((*qt)->binSon[1]),r,zworld->Center[1],zworld->Length[0], 1);
           } else if (
               (zworld->Center[1] <= (r->Center[1] + r->Length[1])) &&
               (zworld->Center[1] >= (r->Center[1] - r->Length[1]))){
                   //intersects x axis
                   (*qt)->binSon[0] = new bNode;
                   AXIS_INSERT(&((*qt)->binSon[0]),r,zworld->Center[0],zworld->Length[0], 0);
               } else if (r->Center[0] < zworld->Center[0] && r->Center[1] > zworld->Center[1]){
                   //q1
                   //modify z 
                   zworld->Center[1] += (zworld->Length[0]/4);
                   zworld->Center[0] -= (zworld->Length[0]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   if ((*qt)->spcSon[0] == nullptr){
                       (*qt)->spcSon[0] = new cNode;
                       (*qt)->spcSon[0]->spcSon[0] = nullptr;
                       (*qt)->spcSon[0]->spcSon[1] = nullptr;
                       (*qt)->spcSon[0]->spcSon[2] = nullptr;
                       (*qt)->spcSon[0]->spcSon[3] = nullptr;
                       (*qt)->spcSon[0]->binSon[1] = nullptr;
                       (*qt)->spcSon[0]->binSon[0] = nullptr;
                   }
                   REC_INSERT(r,&((*qt)->spcSon[0]),zworld);
                   //recall function
               } else if (r->Center[0] > zworld->Center[0] && r->Center[1] > zworld->Center[1]){
                   //q2
                   zworld->Center[0] += (zworld->Length[0]/4);
                   zworld->Center[1] += (zworld->Length[0]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);


                   if ((*qt)->spcSon[1] == nullptr){
                       (*qt)->spcSon[1] = new cNode;
                       (*qt)->spcSon[1]->spcSon[0] = nullptr;
                       (*qt)->spcSon[1]->spcSon[1] = nullptr;
                       (*qt)->spcSon[1]->spcSon[2] = nullptr;
                       (*qt)->spcSon[1]->spcSon[3] = nullptr;
                       (*qt)->spcSon[1]->binSon[1] = nullptr;
                       (*qt)->spcSon[1]->binSon[0] = nullptr;
                   }
                   REC_INSERT(r,&((*qt)->spcSon[1]),zworld); 
               } else if (r->Center[0] < zworld->Center[0] && r->Center[1] < zworld->Center[1]){
                   //q3
                   zworld->Center[0] -= (zworld->Length[0]/4);
                   zworld->Center[1] -= (zworld->Length[1]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                                      
                   if ((*qt)->spcSon[2] == nullptr){
                       (*qt)->spcSon[2] = new cNode;
                       (*qt)->spcSon[2]->spcSon[0] = nullptr;
                       (*qt)->spcSon[2]->spcSon[1] = nullptr;
                       (*qt)->spcSon[2]->spcSon[2] = nullptr;
                       (*qt)->spcSon[2]->spcSon[3] = nullptr;
                       (*qt)->spcSon[2]->binSon[1] = nullptr;
                       (*qt)->spcSon[2]->binSon[0] = nullptr;
                   }
                   REC_INSERT(r,&((*qt)->spcSon[2]),zworld); 
               } else if (r->Center[0] > zworld->Center[0] && r->Center[1] < zworld->Center[1]){
                   //q4
                   zworld->Center[0] += (zworld->Length[0]/4);
                   zworld->Center[1] -= (zworld->Length[1]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   
                   if ((*qt)->spcSon[3] == nullptr){
                       (*qt)->spcSon[3] = new cNode;
                       (*qt)->spcSon[3]->spcSon[0] = nullptr;
                       (*qt)->spcSon[3]->spcSon[1] = nullptr;
                       (*qt)->spcSon[3]->spcSon[2] = nullptr;
                       (*qt)->spcSon[3]->spcSon[3] = nullptr;
                       (*qt)->spcSon[3]->binSon[1] = nullptr;
                       (*qt)->spcSon[3]->binSon[0] = nullptr;
                   }
                   REC_INSERT(r,&((*qt)->spcSon[3]),zworld); 
               } else {
                   cout<<"broke";
               }        
}
string bSEARCH_POINT(int x, int y){
    if (chead == nullptr){
       return "";
    } else{
        checkBoard* temp = chead;
        while(temp->next != nullptr){
            if (temp->x == x && temp->y == y){
                return temp->Name;
            }
            temp=temp->next;
        }
        
    }
    return "";
}

void INSERT(string R){
    recTree->Length[0] = wl;
    recTree->Length[1] = wl;
    recTree->Center[0] = (wl/2);
    recTree->Center[1] = (wl/2);
    struct Rectangle * rec = find_in_collection(R,&colHead);
    if (rec != nullptr){
        //cout<<rec->Name;
    } else {
         cout<<" rectangle not created";
         return;
    }
    if (((rec->Center[0] + (rec->Length[0])) > recTree->Length[0]) ||
        rec->Center[0] - (rec->Length[0]) <= 0 ||
        ((rec->Center[1] + (rec->Length[1])) > recTree->Length[1]) ||
        (rec->Center[1] - (rec->Length[1]) <= 0))
    {
        cout<<"INSERTION OF RECTANGLE N FAILED AS N LIES PARTIALLY OUTSIDE SPACE SPANNED BY MX-CIF QUADTREE";
        return;
    }
    for (int x = (rec->Center[0] - (rec->Length[0])+1); x < (rec->Center[0] + (rec->Length[0]));x++){
        for (int y = (rec->Center[1] - (rec->Length[1])+1); y < (rec->Center[1] + (rec->Length[1]));y++){
            string s = bSEARCH_POINT(x,y);
            if (s != ""){
                cout<<"intersects with ";
                cout<<s;
                return;
            }
        }
    }
    REC_INSERT(rec, &mxCifTree.mxCifRoot, recTree);
}

void SEARCH_POINT(int x, int y){
    string b = bSEARCH_POINT(x,y);
    if (b != ""){
        cout<<"found rectangle ";
        cout<<b;
    } else{
        cout<<"no rectangle found";
    }
}

void RECTANGLE_SEARCH(string r){
    struct Rectangle * rec = find_in_collection(r,&colHead);
    if (rec == nullptr) {
         cout<<" rectangle not created";
         return;
    }
    checkBoard* temp = chead;
        while(temp->next != nullptr){
            if (temp->x >= (rec->Center[0])-(rec->Length[0]) && temp->x <= (rec->Center[0])+(rec->Length[0])
                && temp->y >= (rec->Center[1])-(rec->Length[1]) && temp->y <= (rec->Center[1])+(rec->Length[1])){
                cout<<rec->Name;
                cout<<" INTERSECTS RECTANGLE ";
                cout<<temp->Name;
                return;
            }
            temp=temp->next;
        }
    cout<<rec->Name;
    cout<<" DOES NOT INTERSECT AN EXISTING RECTANGLE";
}

void C_LIST_DELETE(int x, int y){
    if (chead == nullptr){
        return;
    } else{
        checkBoard* temp = chead;
        if (temp->x == x && temp->y == y){
            chead = chead->next;
        }

        while(temp->next != nullptr){
            temp=temp->next;
            if (temp->x == x && temp->y == y){
                if (temp->next != nullptr && temp->prev != nullptr){
                  temp->prev->next = temp->next;
                  return;
                } else if (temp->prev != nullptr){
                    temp->prev->next = nullptr;
                } else {
                    chead = temp->next;
                }
            }
            //temp=temp->next;
        }
    }
}

void AXIS_DELETE(bNode ** bt, Rectangle * r, int cent, int len, int axis){
    if (cent > r->Center[axis]+r->Length[axis]){
            //go left
            if ((*bt)->binSon[0] == nullptr){
                (*bt)->binSon[0] = new bNode;
                (*bt)->binSon[0]->binSon[0] = nullptr;
                (*bt)->binSon[0]->binSon[1] = nullptr;
                (*bt)->binSon[0]->rect = nullptr;
            }
            AXIS_DELETE(&((*bt)->binSon[0]),r,(cent - (len/4)),(len/2),axis);
        } else if (cent < r->Center[axis]-r->Length[axis]){
            //go right
            if ((*bt)->binSon[1] == nullptr){
                (*bt)->binSon[1] = new bNode;
                (*bt)->binSon[1]->binSon[0] = nullptr;
                (*bt)->binSon[1]->binSon[1] = nullptr;
                (*bt)->binSon[1]->rect = nullptr;
            }
            AXIS_DELETE(&((*bt)->binSon[1]),r,(cent + (len/4)),(len/2),axis);
        } else{
            //delete
            cout<<(*bt)->rect->Name;
            cout<<" deleted";
           //(*bt)->rect->Name = nullptr; 
            for (int x = (r->Center[0] - (r->Length[0]));x <= (r->Center[0] + (r->Length[0])); x++){
                    for (int y = (r->Center[1] - (r->Length[1]));y <= (r->Center[1] + (r->Length[1])); y++){
                        //cout<<"p";
                        C_LIST_DELETE(x,y);
                }
            }
            (*bt)->rect = nullptr;
            bt = nullptr; 
 } 
}

void REC_DELETE(Rectangle * r, cNode ** qt, Rectangle * zworld){
    if (((zworld->Center[0]) <= (r->Center[0] + r->Length[0])) &&
       (zworld->Center[0] >= (r->Center[0] - r->Length[0])) &&
       (zworld->Center[1] <= (r->Center[1] + r->Length[1])) &&
       (zworld->Center[1] >= (r->Center[1] - r->Length[1])))
       {
           for (int x = (r->Center[0] - (r->Length[0]));x <= (r->Center[0] + (r->Length[0])); x++){
                    for (int y = (r->Center[1] - (r->Length[1]));y <= (r->Center[1] + (r->Length[1])); y++){
                        C_LIST_DELETE(x,y);
                }
            }
           cout<<"deleted origin rectangle ";
           cout<<r->Name;
           (*qt)->binSon[1]->rect = nullptr;
           (*qt)->binSon[1] = nullptr;
           return;

       } else if (
           (zworld->Center[0] <= (r->Center[0] + r->Length[0])) &&
           (zworld->Center[0] >= (r->Center[0] - r->Length[0]))){
               //itersects y axis (call seprate function)
               //(*qt)->binSon[1] = new bNode;
                if  ((*qt)->binSon[0] == nullptr){
                    cout<<"deleted rectangle ";
                    cout<<r->Name;
                    return;
                } 
               AXIS_DELETE(&(*qt)->binSon[1],r,zworld->Center[1],zworld->Length[0], 1);
               //axis delete
           } else if (
               (zworld->Center[1] <= (r->Center[1] + r->Length[1])) &&
               (zworld->Center[1] >= (r->Center[1] - r->Length[1]))){
                   //intersects x axis
                    if  ((*qt)->binSon[0] == nullptr){
                        cout<<"deleted rectangle ";
                        cout<<r->Name;
                        return;
                    }
                AXIS_DELETE(&(*qt)->binSon[0],r,zworld->Center[0],zworld->Length[0], 0);

                
                   //axis delete
               } else if (r->Center[0] < zworld->Center[0] && r->Center[1] > zworld->Center[1]){
                   //q1
                   //modify z 
                   zworld->Center[1] += (zworld->Length[0]/4);
                   zworld->Center[0] -= (zworld->Length[0]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   REC_DELETE(r,&((*qt)->spcSon[0]),zworld);
                   //recall function
               } else if (r->Center[0] > zworld->Center[0] && r->Center[1] > zworld->Center[1]){
                   //q2
                   zworld->Center[0] += (zworld->Length[0]/4);
                   zworld->Center[1] += (zworld->Length[0]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   REC_DELETE(r,&((*qt)->spcSon[1]),zworld); 
               } else if (r->Center[0] < zworld->Center[0] && r->Center[1] < zworld->Center[1]){
                   //q3
                   zworld->Center[0] -= (zworld->Length[0]/4);
                   zworld->Center[1] -= (zworld->Length[1]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   REC_DELETE(r,&((*qt)->spcSon[2]),zworld); 
               } else if (r->Center[0] > zworld->Center[0] && r->Center[1] < zworld->Center[1]){
                   //q4
                   zworld->Center[0] += (zworld->Length[0]/4);
                   zworld->Center[1] -= (zworld->Length[1]/4);
                   zworld->Length[0] = (zworld->Length[0]/2);
                   zworld->Length[1] = (zworld->Length[1]/2);
                   REC_DELETE(r,&((*qt)->spcSon[3]),zworld); 
               } else {
                   cout<<"broke";
               }     
}

void DELETE_RECTANGLE(string r){
    recTree->Length[0] = wl;
    recTree->Length[1] = wl;
    recTree->Center[0] = (wl/2);
    recTree->Center[1] = (wl/2);
    struct Rectangle * rec = find_in_collection(r,&colHead);
    if (rec == nullptr) {
         cout<<"rectangle not created";
         return;
    }
    if (bSEARCH_POINT(rec->Center[0],rec->Center[1]) == ""){
        cout<<"rectangle not inserted";
        return;
    }
    REC_DELETE(rec, &mxCifTree.mxCifRoot, recTree);

}
void DELETE_POINT(int x, int y){
    if (chead == nullptr){
        cout<<"no rectangle found";
    } else{
        checkBoard* temp = chead;
        while(temp->next != nullptr){
            if (temp->x == x && temp->y == y){
                DELETE_RECTANGLE(temp->Name);
                return;
            }
            temp=temp->next;
        }
        
    }
    cout<<"no rectangle found";
}

void MOVE(string name, int xv, int yv){
    struct Rectangle * rec = find_in_collection(name,&colHead);
        if (rec == nullptr) {
            cout<<"rectangle not created";
            return;
        }
    Rectangle * r = new Rectangle;
    r->Center[0] = rec->Center[0] + xv;
    r->Center[1] = rec->Center[1] + xv;
    for(int i =0; i< 7;i++){
        r->Name[i] = rec->Name[i];
    } 


}
void REC_ARC_BTREE(bNode ** b){


    if ((*b)->binSon[0] != nullptr){
        REC_ARC_BTREE(&(*b)->binSon[0]);
    }
    if ((*b)->binSon[0] == nullptr && (*b)->binSon[1] != nullptr){
        cout<<"W2 ";
    } else{
    cout<<"G2 ";
    }
    if ((*b)->rect != nullptr){
        cout<<(*b)->rect->Name;
    }
    if ((*b)->binSon[1] != nullptr){
        REC_ARC_BTREE(&(*b)->binSon[1]);
    }
}

void REC_ARC_QTREE(cNode ** c){

    if ((*c)->binSon[0] != nullptr){
        REC_ARC_BTREE(&(*c)->binSon[0]);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_ARC_BTREE(&(*c)->binSon[1]);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_ARC_QTREE(&(*c)->spcSon[0]);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_ARC_QTREE(&(*c)->spcSon[1]);
    }
    if ((*c)->spcSon[0] == nullptr && (*c)->spcSon[1] == nullptr && (*c)->spcSon[2] == nullptr &&
        (*c)->spcSon[3] == nullptr ){
            cout<<"W4 ";
    } else{
    cout<<"G4 ";
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_ARC_QTREE(&(*c)->spcSon[2]);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_ARC_QTREE(&(*c)->spcSon[3]);
    }
}
void ARCHIVE_QUADTREE(){
    REC_ARC_QTREE(&mxCifTree.mxCifRoot);
}


int t = 0;
string tnames = " ";
void REC_TOUCH_BTREE(bNode ** b, Rectangle * name){
    
    if ((*b)->binSon[0] != nullptr){
        REC_TOUCH_BTREE(&(*b)->binSon[0],name);
    }
    if ((*b)->rect != nullptr){
        if ((*b)->rect->Name != name->Name){
            if ((((*b)->rect->Center[0] + (*b)->rect->Length[0]) == (name->Center[0] - name->Length[0]) &&
                ((*b)->rect->Center[1] + (*b)->rect->Length[1]) > (name->Center[1] - name->Length[1]) &&
                ((*b)->rect->Center[1] - (*b)->rect->Length[1]) < (name->Center[1] + name->Length[1]))
                ||
                (((*b)->rect->Center[0] - (*b)->rect->Length[0]) == (name->Center[0] + name->Length[0]) &&
                ((*b)->rect->Center[1] + (*b)->rect->Length[1]) > (name->Center[1] - name->Length[1]) &&
                ((*b)->rect->Center[1] - (*b)->rect->Length[1]) < (name->Center[1] + name->Length[1]))
                ||
                (((*b)->rect->Center[1] + (*b)->rect->Length[1]) == (name->Center[1] - name->Length[0]) &&
                ((*b)->rect->Center[0] + (*b)->rect->Length[0]) > (name->Center[0] - name->Length[0]) &&
                ((*b)->rect->Center[0] - (*b)->rect->Length[0]) < (name->Center[0] + name->Length[0]))
                ||
                (((*b)->rect->Center[1] - (*b)->rect->Length[0]) == (name->Center[1] + name->Length[0]) &&
                ((*b)->rect->Center[0] + (*b)->rect->Length[1]) > (name->Center[0] - name->Length[1]) &&
                ((*b)->rect->Center[0] - (*b)->rect->Length[1]) < (name->Center[0] + name->Length[1]))
            )  
                {
                    t++;
                    //tnames.push_back((*b)->rect->Name);
                    tnames = tnames+(*b)->rect->Name;
                    tnames.push_back(' ');
            }
        } 
    }

    if ((*b)->binSon[1] != nullptr){
        REC_TOUCH_BTREE(&(*b)->binSon[1],name);
    }

}


void REC_TOUCH_QTREE(cNode ** c, Rectangle * name){
    if ((*c)->binSon[0] != nullptr){
        REC_TOUCH_BTREE(&(*c)->binSon[0],name);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_TOUCH_BTREE(&(*c)->binSon[1],name);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_TOUCH_QTREE(&(*c)->spcSon[0],name);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_TOUCH_QTREE(&(*c)->spcSon[1],name);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_TOUCH_QTREE(&(*c)->spcSon[2],name);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_TOUCH_QTREE(&(*c)->spcSon[3],name);
    }
}

void TOUCH(string name){
    struct Rectangle * rec = find_in_collection(name,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return;
        }
    t = 0;
    tnames = " ";
    REC_TOUCH_QTREE(&mxCifTree.mxCifRoot,rec);
    if (t != 0){
    cout<<"found ";
    cout<<t;
    if (t > 1){
        cout<<" rectangles: ";
    } else {
    cout<<" rectangle: ";
    }
    cout<<tnames;
    }else {
        cout<<"no rectangle found";
    }
}

int w = 0;
string wnames = " ";
void REC_WITHIN_BTREE(bNode ** b, Rectangle * name,int d){
    if ((*b)->binSon[0] != nullptr){
        REC_WITHIN_BTREE(&(*b)->binSon[0],name,d);
    }
    // bottom left
    if ((*b)->rect != nullptr){
    if ((*b)->rect->Name != name->Name){
        //within large rectangle
        if (((((*b)->rect->Center[0] + (*b)->rect->Length[0])) >= (name->Center[0] - name->Length[0] - d) &&
        ((*b)->rect->Center[0] - (*b)->rect->Length[0]) <= (name->Center[0] + name->Length[0] + d) &&
        ((*b)->rect->Center[1] + (*b)->rect->Length[1]) >= (name->Center[1] - name->Length[1] - d) &&
        ((*b)->rect->Center[1] - (*b)->rect->Length[1]) <= (name->Center[1] + name->Length[1] + d))){
            //not entierly in small rectangle
            if (((*b)->rect->Center[0] + (*b)->rect->Length[0]) > name->Center[0] + name->Length[0] ||
            ((*b)->rect->Center[0] - (*b)->rect->Length[0]) < name->Center[0] - name->Length[0] ||
            ((*b)->rect->Center[1] + (*b)->rect->Length[1]) > name->Center[1] + name->Length[1] ||
            ((*b)->rect->Center[1] - (*b)->rect->Length[1]) > name->Center[1] - name->Length[1]
            ){
            w++;
            wnames = wnames+(*b)->rect->Name;
            wnames.push_back(' ');
            }
        }
    }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_WITHIN_BTREE(&(*b)->binSon[1],name,d);
    }

    
}

void REC_WITHIN_QTREE(cNode ** c, Rectangle * name, int d){
    if ((*c)->binSon[0] != nullptr){
        REC_WITHIN_BTREE(&(*c)->binSon[0],name,d);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_WITHIN_BTREE(&(*c)->binSon[1],name,d);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_WITHIN_QTREE(&(*c)->spcSon[0],name,d);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_WITHIN_QTREE(&(*c)->spcSon[1],name,d);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_WITHIN_QTREE(&(*c)->spcSon[2],name,d);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_WITHIN_QTREE(&(*c)->spcSon[3],name,d);
    }
}

void WITHIN(string n, int d){
    struct Rectangle * rec = find_in_collection(n,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return;
        }
    w = 0;
    wnames = " ";
    REC_WITHIN_QTREE(&mxCifTree.mxCifRoot,rec,d);
    if (w>0){
        if (w=1){
            cout<<"found 1 rectangle: ";
        }else{
            cout<<"found ";
            cout<<w;
            cout<<" rectangles: ";
        }
        cout<<wnames;
    } else{
        cout<<"no rectangle found";
    }
}

int hd =0;
string hname = " ";
void REC_HORIZ_BTREE(bNode ** b, Rectangle * name){
    if ((*b)->binSon[0] != nullptr){
        REC_HORIZ_BTREE(&(*b)->binSon[0],name);
    }
    ///////////////////
    if ((*b)->rect != nullptr){
    if ((*b)->rect->Name != name->Name){
        if ((*b)->rect->Center[0] - (*b)->rect->Length[0] >= name->Center[0] + name->Length[0]){
             if ((((*b)->rect->Center[0] - (*b)->rect->Length[0]) - (name->Center[0] + name->Length[0])) < hd){
                 hd = (((*b)->rect->Center[0] - (*b)->rect->Length[0]) - (name->Center[0] + name->Length[0]));
                 hname = (*b)->rect->Name;
        }
        } else if ((*b)->rect->Center[0] + (*b)->rect->Length[0] <= name->Center[0] - name->Length[0]){
            if (((name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0])) < hd){
                hd = ((name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]));
                hname = (*b)->rect->Name;
        }
        }
    }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_HORIZ_BTREE(&(*b)->binSon[1],name);
    }
}

void REC_HORIZ_QTREE(cNode ** c, Rectangle * name){
    if ((*c)->binSon[0] != nullptr){
        REC_HORIZ_BTREE(&(*c)->binSon[0],name);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_HORIZ_BTREE(&(*c)->binSon[1],name);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_HORIZ_QTREE(&(*c)->spcSon[0],name);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_HORIZ_QTREE(&(*c)->spcSon[1],name);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_HORIZ_QTREE(&(*c)->spcSon[2],name);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_HORIZ_QTREE(&(*c)->spcSon[3],name);
    }
}

void HORIZ_NEIGHBOR(string n){
    if (recTree != nullptr){
    hd = 2 * recTree->Length[0];
    }
    struct Rectangle * rec = find_in_collection(n,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return;
        }
    
    REC_HORIZ_QTREE(&mxCifTree.mxCifRoot,rec);
    cout<<"found rectangle ";
    cout<<hname;
}

int vd = 0;
string vname = " ";
void REC_VERT_BTREE(bNode ** b, Rectangle * name){
    if ((*b)->binSon[0] != nullptr){
        REC_VERT_BTREE(&(*b)->binSon[0],name);
    }
    if ((*b)->rect != nullptr){
    if ((*b)->rect->Name != name->Name){

        if ((*b)->rect->Center[1] - (*b)->rect->Length[1] >= name->Center[1] + name->Length[1]){
             if ((((*b)->rect->Center[1] - (*b)->rect->Length[1]) - (name->Center[1] + name->Length[1])) < vd){
                 vd = (((*b)->rect->Center[1] - (*b)->rect->Length[1]) - (name->Center[1] + name->Length[1]));
                 vname = (*b)->rect->Name;
        }
        } else if ((*b)->rect->Center[1] + (*b)->rect->Length[1] <= name->Center[1] - name->Length[1]){
            if (((name->Center[1] - name->Length[1]) - ((*b)->rect->Center[1] + (*b)->rect->Length[1])) < vd){
                vd = ((name->Center[1] - name->Length[1]) - ((*b)->rect->Center[1] + (*b)->rect->Length[1]));
                vname = (*b)->rect->Name;
        }
        }
    }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_HORIZ_BTREE(&(*b)->binSon[1],name);
    }
}

void REC_VERT_QTREE(cNode ** c, Rectangle * name){
    if ((*c)->binSon[0] != nullptr){
        REC_VERT_BTREE(&(*c)->binSon[0],name);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_VERT_BTREE(&(*c)->binSon[1],name);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_VERT_QTREE(&(*c)->spcSon[0],name);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_VERT_QTREE(&(*c)->spcSon[1],name);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_VERT_QTREE(&(*c)->spcSon[2],name);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_VERT_QTREE(&(*c)->spcSon[3],name);
    }
}

void VERT_NEIGHBOR(string n){
    if (recTree != nullptr){
    vd = 2 * recTree->Length[0];
    }
    struct Rectangle * rec = find_in_collection(n,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return;
        }
    REC_VERT_QTREE(&mxCifTree.mxCifRoot,rec);
    cout<<"found rectangle ";
    cout<<vname;
    
}
int nd =0;
string nname = " ";
void REC_NEAR_BTREE(bNode ** b,int x,int y){
    if ((*b)->binSon[0] != nullptr){
        REC_NEAR_BTREE(&(*b)->binSon[0],x,y);
    }
    int dx = 0;
    int dy = 0;
    if ((*b)->rect != nullptr){
        //N
        if (y > ((*b)->rect->Center[1] + (*b)->rect->Length[1]) &&
         x <= ((*b)->rect->Center[0] + (*b)->rect->Length[0]) &&
          x >= ((*b)->rect->Center[0] - (*b)->rect->Length[0])){
            if (y - ((*b)->rect->Center[1] + (*b)->rect->Length[1]) < nd){
                nd = y - ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
                nname = (*b)->rect->Name;
            }
        //NE
        } else if (y > ((*b)->rect->Center[1] + (*b)->rect->Length[1]) &&
         x > ((*b)->rect->Center[0] + (*b)->rect->Length[0])){
            dx = x - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
            dy = y - ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
            if (sqrt((dx^2)+(dy^2)) < nd){
                nd = sqrt((dx^2)+(dy^2));
                nname = (*b)->rect->Name;
            }
        }
        //E
        else if (X > ((*b)->rect->Center[0] + (*b)->rect->Length[0]) &&
         y <= ((*b)->rect->Center[1] + (*b)->rect->Length[1]) &&
          y >= ((*b)->rect->Center[1] - (*b)->rect->Length[1])){
            if (x - ((*b)->rect->Center[0] + (*b)->rect->Length[0]) < nd){
                nd = x - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                nname = (*b)->rect->Name;
            }
        }
        //SE
        else if (y < ((*b)->rect->Center[1] - (*b)->rect->Length[1]) &&
         x > ((*b)->rect->Center[0] + (*b)->rect->Length[0])){
            dx = x - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
            dy = ((*b)->rect->Center[1] - (*b)->rect->Length[1]) - y;
            if (sqrt((dx^2)+(dy^2)) < nd){
                nd = sqrt((dx^2)+(dy^2));
                nname = (*b)->rect->Name;
            }
        }
        //S
        else if (y < ((*b)->rect->Center[1] - (*b)->rect->Length[1]) &&
         x <= ((*b)->rect->Center[0] + (*b)->rect->Length[0]) &&
          x >= ((*b)->rect->Center[0] - (*b)->rect->Length[0])){
            if (((*b)->rect->Center[1] - (*b)->rect->Length[1]) - y < nd){
                nd =  ((*b)->rect->Center[1] + (*b)->rect->Length[1]) - y;
                nname = (*b)->rect->Name;
        }
        //SW
        } else if (y < ((*b)->rect->Center[1] - (*b)->rect->Length[1]) &&
         x < ((*b)->rect->Center[0] - (*b)->rect->Length[0])){
            dx =  ((*b)->rect->Center[0] - (*b)->rect->Length[0]) - x;
            dy = ((*b)->rect->Center[1] - (*b)->rect->Length[1]) - y;
            if (sqrt((dx^2)+(dy^2)) < nd){
                nd = sqrt((dx^2)+(dy^2));
                nname = (*b)->rect->Name;
            }
        }
        //W
         else if (x < ((*b)->rect->Center[0] - (*b)->rect->Length[0]) &&
          y <= ((*b)->rect->Center[1] + (*b)->rect->Length[1]) &&
           y >= ((*b)->rect->Center[1] - (*b)->rect->Length[1])){
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0]) - x < nd){
                nd =  ((*b)->rect->Center[0] - (*b)->rect->Length[0]) - x;
                nname = (*b)->rect->Name;
        }
        //NW
        } else if (y > ((*b)->rect->Center[1] + (*b)->rect->Length[1]) &&
         x < ((*b)->rect->Center[0] - (*b)->rect->Length[0])){
            dx = ((*b)->rect->Center[0] - (*b)->rect->Length[0]) - x;
            dy = y - ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
            if (sqrt((dx^2)+(dy^2)) < nd){
                nd = sqrt((dx^2)+(dy^2));
                nname = (*b)->rect->Name;
            }
        }
        else{
            nd = 0;
            nname = (*b)->rect->Name;
        }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_NEAR_BTREE(&(*b)->binSon[1],x,y);
    }
}

void REC_NEAR_QTREE(cNode ** c, int x, int y){
    if ((*c)->binSon[0] != nullptr){
        REC_NEAR_BTREE(&(*c)->binSon[0],x,y);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_NEAR_BTREE(&(*c)->binSon[1],x,y);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_NEAR_QTREE(&(*c)->spcSon[0],x,y);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_NEAR_QTREE(&(*c)->spcSon[1],x,y);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_NEAR_QTREE(&(*c)->spcSon[2],x,y);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_NEAR_QTREE(&(*c)->spcSon[3],x,y);
    }
}
void NEAREST_RECTANGLE(int x, int y){

    if (recTree != nullptr){
        nd = (2*recTree->Length[0]) + (2*recTree->Length[1]);
    }
    REC_NEAR_QTREE(&mxCifTree.mxCifRoot,x,y);
    cout<<"found ";
    cout<<nname;
}
string winames;
int wincount;
void REC_BWINDOW(bNode ** b,int x, int y, int w, int h){
    if ((*b)->binSon[0] != nullptr){
        REC_BWINDOW(&(*b)->binSon[0],x,y,w,h);
    }
    if ((*b)->rect != nullptr){
        if ((*b)->rect->Center[0] - (*b)->rect->Length[0] >= x && (*b)->rect->Center[0] + (*b)->rect->Length[0] <= (x+w) &&
        (*b)->rect->Center[1] - (*b)->rect->Length[1] >= y && (*b)->rect->Center[1] - (*b)->rect->Length[1] <= (y+h)){
            winames = winames+(*b)->rect->Name;
            winames.push_back(' ');
            wincount++;
        }
    }

    if ((*b)->binSon[1] != nullptr){
        REC_BWINDOW(&(*b)->binSon[1],x,y,w,h);
    }
}

void REC_QWINDOW(cNode ** c, int x, int y, int w, int h){
    if ((*c)->binSon[0] != nullptr){
        REC_BWINDOW(&(*c)->binSon[0],x,y,w,h);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_BWINDOW(&(*c)->binSon[1],x,y,w,h);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_QWINDOW(&(*c)->spcSon[0],x,y,w,h);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_QWINDOW(&(*c)->spcSon[1],x,y,w,h);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_QWINDOW(&(*c)->spcSon[2],x,y,w,h);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_QWINDOW(&(*c)->spcSon[3],x,y,w,h);
    }
}

void WINDOW(int x, int y, int w, int h){
    wincount = 0;
    REC_QWINDOW(&mxCifTree.mxCifRoot,x,y,w,h);
    if (wincount > 0){
    cout<<"found ";
    cout<<winames;
    } else {
        cout<<"no rectangle found";
    }
}
int nnd;
string nn;
void REC_BNN(bNode ** b, Rectangle * name){
    if ((*b)->binSon[0] != nullptr){
        REC_BNN(&(*b)->binSon[0],name);
    }
    if ((*b)->rect != nullptr){
        //N
        if ((*b)->rect->Center[1] - (*b)->rect->Length[1] > name->Center[1] + name->Length[1]){
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] - (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]) ||
            (((*b)->rect->Center[0] + (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] + (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]))){
                if ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]) < nnd){
                    nnd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                    nn = (*b)->rect->Name;
                }
                //NE
            } else if ((*b)->rect->Center[0] - (*b)->rect->Length[0] > name->Center[0] + name->Length[0]){
                int vd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                int hd = (*b)->rect->Center[0] - (*b)->rect->Length[0] - (name->Center[0] + name->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < nnd){
                    nnd = sqrt((vd^2)+(hd^2));
                    nn = (*b)->rect->Name;
                }
                //NW
            } else {
                int vd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                int hd = (name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < nnd){
                    nnd = sqrt((vd^2)+(hd^2));
                    nn = (*b)->rect->Name;
                }
            }
            //S
        } else if ((*b)->rect->Center[1] + (*b)->rect->Length[1] < name->Center[1] - name->Length[1]){
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] - (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]) ||
            (((*b)->rect->Center[0] + (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] + (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]))){
                if   (name->Center[1] - name->Length[1] - ((*b)->rect->Center[1] + (*b)->rect->Length[1]) < nnd){
                    nnd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                    nn = (*b)->rect->Name;
                }
                //SE
            } else if ((*b)->rect->Center[0] - (*b)->rect->Length[0] > name->Center[0] + name->Length[0]){
                int vd =  (name->Center[1] - name->Length[1])- ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
                int hd = (*b)->rect->Center[0] - (*b)->rect->Length[0] - (name->Center[0] + name->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < nnd){
                    nnd = sqrt((vd^2)+(hd^2));
                    nn = (*b)->rect->Name;
                }
                //SW
            } else {
                int vd = (name->Center[1] - name->Length[1])- ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
                int hd = (name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < nnd){
                    nnd = sqrt((vd^2)+(hd^2));
                    nn = (*b)->rect->Name;
                }
            }

        } else if ((*b)->rect->Center[0] + (*b)->rect->Length[0] < (name->Center[0]-name->Length[0])){
            //W
            if ((name->Center[0]-name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]) < nnd){
                nnd = (name->Center[0]-name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                nn = (*b)->rect->Name;
            }

        } else{
            //E
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]) < nnd && 
            ((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]) > 0){
                nnd = ((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]);
                nn = (*b)->rect->Name;
            }
        }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_BNN(&(*b)->binSon[1],name);
    }
}

void REC_QNN(cNode ** c, Rectangle * name){
    if ((*c)->binSon[0] != nullptr){
        REC_BNN(&(*c)->binSon[0],name);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_BNN(&(*c)->binSon[1],name);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_QNN(&(*c)->spcSon[0],name);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_QNN(&(*c)->spcSon[1],name);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_QNN(&(*c)->spcSon[2],name);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_QNN(&(*c)->spcSon[3],name);
    }
}

void NEAREST_NEIGHBOR(string name){
    struct Rectangle * rec = find_in_collection(name,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return;
        }
    if (recTree != nullptr){
        nnd = 2 * recTree->Length[0];
    }
    nn = "";
    REC_QNN(&mxCifTree.mxCifRoot,rec);
    if (nn != ""){
    cout<<"found rectangle ";
    cout<<nn;
    } else {
        cout<<"no rectangle found";
    }
}
string lgnn;
int lgnnd;
void REC_LEX_BNN(bNode ** b, Rectangle * name){
    if ((*b)->binSon[0] != nullptr){
        REC_LEX_BNN(&(*b)->binSon[1],name);
    }

    if ((*b)->rect != nullptr){
    bool great = false;
    int i =0;
    while(i<7){
        if ((*b)->rect->Name[i] > name->Name[i]){
            great = true;
            i = 7;
        } else if ((*b)->rect->Name[i] < name->Name[i]){
            i = 7;
        }
        i++;
    }
    if (great == true){
        //N
        if ((*b)->rect->Center[1] - (*b)->rect->Length[1] > name->Center[1] + name->Length[1]){
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] - (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]) ||
            (((*b)->rect->Center[0] + (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] + (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]))){
                if ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]) < lgnnd){
                    lgnnd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                    lgnn = (*b)->rect->Name;
                }
                //NE
            } else if ((*b)->rect->Center[0] - (*b)->rect->Length[0] > name->Center[0] + name->Length[0]){
                int vd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                int hd = (*b)->rect->Center[0] - (*b)->rect->Length[0] - (name->Center[0] + name->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < lgnnd){
                    lgnnd = sqrt((vd^2)+(hd^2));
                    lgnn = (*b)->rect->Name;
                }
                //NW
            } else {
                int vd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                int hd = (name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < lgnnd){
                    lgnnd = sqrt((vd^2)+(hd^2));
                    lgnn = (*b)->rect->Name;
                }
            }
            //S
        } else if ((*b)->rect->Center[1] + (*b)->rect->Length[1] < name->Center[1] - name->Length[1]){
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] - (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]) ||
            (((*b)->rect->Center[0] + (*b)->rect->Length[0] >= name->Center[0] - name->Length[0] &&
            (*b)->rect->Center[0] + (*b)->rect->Length[0] <= name->Center[0] + name->Length[0]))){
                if (name->Center[1] - name->Length[1] - ((*b)->rect->Center[1] + (*b)->rect->Length[1]) < lgnnd){
                    lgnnd = ((*b)->rect->Center[1] - (*b)->rect->Length[1] - (name->Center[1] + name->Length[1]));
                    lgnn = (*b)->rect->Name;
                }
                //SE
            } else if ((*b)->rect->Center[0] - (*b)->rect->Length[0] > name->Center[0] + name->Length[0]){
                int vd =  (name->Center[1] - name->Length[1])- ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
                int hd = (*b)->rect->Center[0] - (*b)->rect->Length[0] - (name->Center[0] + name->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < lgnnd){
                    lgnnd = sqrt((vd^2)+(hd^2));
                    lgnn = (*b)->rect->Name;
                }
                //SW
            } else {
                int vd = (name->Center[1] - name->Length[1])- ((*b)->rect->Center[1] + (*b)->rect->Length[1]);
                int hd = (name->Center[0] - name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                if (sqrt((vd^2)+(hd^2)) < lgnnd){
                    lgnnd = sqrt((vd^2)+(hd^2));
                    lgnn = (*b)->rect->Name;
                }
            }

        } else if ((*b)->rect->Center[0] + (*b)->rect->Length[0] < (name->Center[0]-name->Length[0])){
            //W
            if ((name->Center[0]-name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]) < lgnnd){
                lgnnd = (name->Center[0]-name->Length[0]) - ((*b)->rect->Center[0] + (*b)->rect->Length[0]);
                lgnn = (*b)->rect->Name;
            }

        } else{
            //E
            if (((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]) < lgnnd && 
            ((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]) > 0){
                lgnnd = ((*b)->rect->Center[0] - (*b)->rect->Length[0])-(name->Center[0] + name->Length[0]);
                lgnn = (*b)->rect->Name;
            }
        }
    }
    }
    if ((*b)->binSon[1] != nullptr){
        REC_LEX_BNN(&(*b)->binSon[1],name);
    }
}
void REC_LEX_QNN(cNode ** c, Rectangle * name){
    if ((*c)->binSon[0] != nullptr){
        REC_LEX_BNN(&(*c)->binSon[0],name);
    }
    if ((*c)->binSon[1] != nullptr){
        REC_LEX_BNN(&(*c)->binSon[1],name);
    }
    if ((*c)->spcSon[0] != nullptr){
        REC_LEX_QNN(&(*c)->spcSon[0],name);
    }
    if ((*c)->spcSon[1] != nullptr){
        REC_LEX_QNN(&(*c)->spcSon[1],name);
    }
    if ((*c)->spcSon[2] != nullptr){
        REC_LEX_QNN(&(*c)->spcSon[2],name);
    }
    if ((*c)->spcSon[3] != nullptr){
        REC_LEX_QNN(&(*c)->spcSon[3],name);
    }
}
string LEXICALLY_GREATER_NEAREST_NEIGHBOR(string name){
    struct Rectangle * rec = find_in_collection(name,&colHead);
        if (rec == nullptr) {
            //cout<<"rectangle not created";
            return nullptr;
        }
    if (recTree != nullptr){
        lgnnd = 2 * recTree->Length[0];
    }
    lgnn = "";
    REC_LEX_QNN(&mxCifTree.mxCifRoot,rec);

    if (lgnn == ""){
        cout<<"no rectangle found";
        return "";
    }else{
    cout<<"found rectangle ";
    cout<<lgnn;
    }
    return lgnn;
}

int main(int argc, char* argv[]){
    string cmd;
    cin>>cmd;
    while (cin){
        //cout<<cmd;
        if (cmd.substr(0,3) == "INI"){
            int i = 14;
            string s;
            while (cmd.at(i) != ')'){
                //cout<<cmd.at(i);
                s.push_back(cmd.at(i));
                i++;
            }
            INIT_QUADTREE(stoi(s));
            //cout<<cmd.at(14);
            cout<<"\n";
        } else if (cmd.substr(0,3) == "BUI"){
            int i = 15;
            string s;
            while (cmd.at(i) != ')'){
                //cout<<cmd.at(i);
                s.push_back(cmd.at(i));
                i++;
            }
            cout<<stoi(s);
            INIT_QUADTREE(stoi(s));
        } else if (cmd.substr(0,3) == "ARC"){
            cout<<1;
            cout<<'C';
            cout<<" ";
            ARCHIVE_QUADTREE();
            cout<<"\n";
        } else if (cmd.substr(0,3) == "DIS"){
            cout<<2;
            cout<<"\n";
            DISPLAY();
        } else if (cmd.substr(0,3) == "LIS"){
            LIST_RECTANGLES();
            cout<<"\n";
        }  else if (cmd.substr(0,3) == "CRE"){
            int i = 17;
            char n[7];
            int cx,cy,w,h;
            for(int x =0; x<5; x++){
                string s;
                while(cmd.at(i) != ',' && cmd.at(i) != ')'){
                    s.push_back(cmd.at(i));
                    i++;
                }
                i++;
                if (x == 0){
                for(int ni=0; ni<s.length(); ni++){
                    n[ni] = s.at(ni);
                }
                }
                switch (x){
                    
                    case 1: cx = stoi(s);
                    case 2: cy = stoi(s);
                    case 3: w = stoi(s);
                    case 4: h = stoi(s);         
                }  
                s.clear();
            }
            CREATE_RECTANGLE(n,cx,cy,w,h);
            cout<<"\n";
         }  else if (cmd.substr(0,3) == "SEA"){
            string x;
            string y;
            int i = 13;
            while (cmd.at(i) != ','){
                x.push_back(cmd.at(i));
                i++;
            }
            int px = stoi(x);
            i++;
            while (cmd.at(i) != ')'){
                y.push_back(cmd.at(i));
                i++;
            }
            int py = stoi(y);
            SEARCH_POINT(px,py);
            cout<<"\n";

        }  else if (cmd.substr(0,3) == "REC"){
            int i = 17;
            string s;
            while (cmd.at(i) != ')'){
                s += cmd.at(i);
                i++;
            }
            //cout<<s;
            RECTANGLE_SEARCH(s);
            cout<<"\n";
        }  else if (cmd.substr(0,3) == "INS"){
            int i = 7;
            string s;
            while (cmd.at(i) != ')'){
                s += cmd.at(i);
                i++;
            }
            INSERT(s);
            cout<<"\n";
        }  else if (cmd.substr(0,3) == "DEL"){
            if(cmd.at(7) == 'R'){
                int i = 17;
                string s;
                while (cmd.at(i) != ')'){
                s.push_back(cmd.at(i));
                i++;
                } 
                DELETE_RECTANGLE(s);
            }
            else{
                int i = 13;
                string x;
                string y;
                while (cmd.at(i) != ','){
                x.push_back(cmd.at(i));
                i++;
                } 
                i++;
                while (cmd.at(i) != ')'){
                y.push_back(cmd.at(i));
                i++;
                } 
                DELETE_POINT(stoi(x),stoi(y));
            }
            cout<<"\n";

         }  else if (cmd.substr(0,3) == "MOV"){
             string n;
             string x;
             string y;
             int i = 5;
             while(cmd.at(i) != ','){
                 n.push_back(cmd.at(i));
                 i++;
             }
             i++;
             while(cmd.at(i) != ','){
                 x.push_back(cmd.at(i));
                 i++;
             }
             i++;
             while(cmd.at(i) != ')'){
                 y.push_back(cmd.at(i));
                 i++;
             }
             MOVE(n,stoi(x),stoi(y));
            cout<<"\n";
        }  else if (cmd.substr(0,3) == "TOU"){
            int i = 6;
            string n;
            while (cmd.at(i) != ')'){
                n.push_back(cmd.at(i));
                i++;
            } 
            while (i<13){
                n.push_back(' ');
                i++;
            }
            TOUCH(n);
            cout<<"\n";

        }  else if (cmd.substr(0,3) == "WIT"){
            int i = 7;
            string n;
            string d;
            while (cmd.at(i) != ','){
                n.push_back(cmd.at(i));
                i++;
            } 
            int q = i;
            while (i<14){
                n.push_back(' ');
                i++;
            }
            i = q;
            i++;
            while (cmd.at(i) != ')'){
                d.push_back(cmd.at(i));
                i++;
            } 
            WITHIN(n,stoi(d));
            cout<<"\n";
        
        }  else if (cmd.substr(0,3) == "HOR"){
            int i = 15;
            string n;
            while (cmd.at(i) != ')'){
                n.push_back(cmd.at(i));
                i++;
            }
            while (i<21){
                n.push_back(' ');
                i++;
            }
            HORIZ_NEIGHBOR(n);
            cout<<"\n";
        }  else if (cmd.substr(0,3) == "VER"){
            int i = 14;
            string n;
            while (cmd.at(i) != ')'){
                n.push_back(cmd.at(i));
                i++;
            }
            while (i<21){
                n.push_back(' ');
                i++;
            }
            VERT_NEIGHBOR(n);
            cout<<"\n";
        }  else if (cmd.substr(0,9) == "NEAREST_R"){
            int i = 18;
            string x;
            string y;
            while (cmd.at(i) != ','){
                x.push_back(cmd.at(i));
                i++;
            }
            i++;
            while (cmd.at(i) != ')'){
                y.push_back(cmd.at(i));
                i++;
            }
            int ix = stoi(x);
            int iy = stoi(y);
            NEAREST_RECTANGLE(ix,iy);
            cout<<"\n";
        } else if (cmd.substr(0,3) == "WIN"){
            int i = 7;
            string x = "";
            string y= "";
            string w= "";
            string h= "";
            while (cmd.at(i) != ','){
                x.push_back(cmd.at(i));
                i++;
            }
            i++;
            while (cmd.at(i) != ','){
                y.push_back(cmd.at(i));
                i++;
            }
            i++;
            while (cmd.at(i) != ','){
                w.push_back(cmd.at(i));
                i++;
            }
            i++;
            while (cmd.at(i) != ')'){
                h.push_back(cmd.at(i));
                i++;
            } 
            int ix = stoi(x);
            int iy = stoi(y);
            int iw = stoi(w);
            int ih = stoi(h);
            WINDOW(ix,iy,iw,ih);
            cout<<"\n";

        }   else if (cmd.substr(0,9) == "NEAREST_N"){
            int i = 17;
            string n;
            while (cmd.at(i) != ')'){
                n.push_back(cmd.at(i));
                i++;
            }
            NEAREST_NEIGHBOR(n);
            cout<<"\n";
        }else if (cmd.substr(0,3) == "LEX"){
            int i = 35;
            string n;
            while (cmd.at(i) != ')'){
                n.push_back(cmd.at(i));
                i++;
            }
            LEXICALLY_GREATER_NEAREST_NEIGHBOR(n);
            cout<<"\n";
        } else if (cmd.substr(0,3) == "LAB"){
            cout<<17;
            cout<<" ";
        } else if (cmd.substr(0,3) == "SPA"){
            cout<<18;
            cout<<" ";
        }
        cin>>cmd;
    }
};