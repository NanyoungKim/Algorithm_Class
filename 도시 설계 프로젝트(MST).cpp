//
//  004_12161532_KimNanyoung.cpp
//  2020-1_알고리즘_설계과제(1)
//
//  Created by 김난영 on 2020/05/26.
//  Copyright © 2020 KimNanyoung. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <list>
#include <algorithm>
#include <time.h>

using namespace std;

int N;       //#vertex : 설립된 도시들이 개수 (2<=N <= 50000)
int M;      //#edge : 도시 사이의 비포장 도로 개수 (1 <=M <= 500000)
int q;      //질의 수

int cityNum;        //도시 번호
string cityName;    //도시 명
int cityPeople;     //도시의 인구 수

int city1Num;
int city2Num;
int Distance;

int edgeCnt=0;


class HT;



class Vertex{       //vertex 정보를 저장하는 클래스
  
public:
    int c_num;
    string c_name;
    int c_people;
    int second;
    int secondSize;
    
    Vertex* next;       //연결된 다음 vertex를 가리키는 포인터
   // HT *htPtr;
    
    
    
    HT *htptr;
    
    Vertex(){  }
    
    Vertex(int c_num, string c_name, int c_people){
        this->c_num = c_num;
        this->c_name = c_name;
        this->c_people = c_people;
       
   
    
        this->next = NULL;
    }
    
    
    
    
    
 
};
Vertex* varr[50000] = {0,};        //varr[i] 는 도시번호 i인 vertex에 대한 정보가 저장되어있다.


class HT{       //head와 tail 을 담고있는 객체

public:

    Vertex *head;
    Vertex *tail;

    int edgesuminSet;         //set에 속한 edge의 weight의 합
    int vertexsuminList;      //set에 속한 vertex의 갯수
};



class Edge{
    
public:
    int city1_num;
    int city2_num;
    int distance;
    //Vertex *v;
    Edge();
    Edge(int city1_num, int city2_num, int distance){
        this->city1_num = city1_num;
        this->city2_num = city2_num;
        this->distance = distance;
    }
    
    bool operator < (const Edge &e) const{                  //클래스 객체끼리 비교하는 걸로 바꿔줌
        
        bool res = false;
        int p1 = varr[this->city1_num]->c_people + varr[this->city2_num]->c_people;
        int p2 = varr[e.city1_num]->c_people + varr[e.city2_num]->c_people;
        
        int min = this->city1_num;
        
        
        
        if(this->distance == e.distance){               //edge weight 가 같은 경우
            if(p1 > p2){                                //도시 인구 수의 합이 다르면
                
                res = true;
            }
            else if(p1 < p2){
                res = false;
                
            }
            else{                                    //도시 인구 수의 합이 같으면
                
                if((min > e.city1_num) || (min > e.city2_num)){
                    res = false;
                }
                else{
                    res = true;
                }
                
                
            }
        }
        else if(this->distance > e.distance){
            res = false;
        }
        else{
            res = true;
        }
        
        
        return res;
    }
    
    
};




vector<Edge> edgeVec;       //모든 edge들에 대한 정보 저장
vector<Edge> edgeF;         //선택된 edge들 정보 저장. 벡터 크기가 n-1이 되면 MST 완성
int edgeFcnt = 0;

int chk = 0;

class setList{
    
    
public:
    
    int weightSum = 0;
    int edgeF = 0;

    void makeSet(int cnum, string cname, int cpeople){
        
        HT* ht = new HT;                                        //새로운 HT 객체 생성. HT에는 head, tail 정보를 갖고있다.
        ht->head = new Vertex(cnum, cname, cpeople);            //vertex(도시) 생성
        
        ht->tail = ht->head;                                    //vertex가 한개이므로 tail도 head가 가리키는 vertex를 가리킨다.
        varr[cnum] = ht->head;                                  //배열에 vertex에 대한 정보 저장. index는 도시 번호를 뜻한다.
        
//        ht->head->c_num = cnum;
//        ht->head->c_name = cname;
//        ht->head->c_people = cpeople;
        
        
        ht->head->htptr = ht;
        ht->head->next = NULL;
        
        ht->edgesuminSet = 0;
        ht->vertexsuminList = 1;                                   //처음에는 vertex가 한 개밖에 없다.
        
       
    }
    
    
    
    int sizeList(Vertex *v){                                      //해당 vertex가 포함된 set의 크기 리턴
        
        int cnt = v->htptr->vertexsuminList;                        //cnt : v가 포함된 set의 vertex들의 개수의 합
        
        while(v->htptr->head->next != NULL){                        //해당 vertex가 포함된 set의 리더 노드부터 끝 노드까지 카운트
            cnt++;
            v->htptr->head->next = v->htptr->head->next->next;      //카운트를 했으면 다음 vertex를 가리키게 한다.
        }
        return cnt;
    }
    
    
    int find(Vertex *v){                    //해당 vertex가 포함된 set의 leader Id 을 리턴하는  함수
        return (v->htptr->head->c_num);     //HT의 header가 가리키는 vertex(leader)의 도시번호 리턴
    }
    
    
    Vertex* Union(Vertex *v1, Vertex *v2){         //각 vertex가 속한 set의 leader 비교후 Union 실행
        
        chk = 0;
        
        if(find(v1) == find(v2)){                   //set의 리더가 같으면 Union 실패.
            return NULL;
        }
        
        else{
        HT *set1 = v1->htptr;
        HT *set2 = v2->htptr;
        
        Vertex *cur;
        Vertex* leaderV;
     
        
        
        //Union 을 한 것은 set이 갱신된 것이므로 set의 wieghtSum, set의 크기를 갱신해야한다.
        
        int vsum1, vsum2, vsum, esum1, esum2, esum = 0;
        vsum1 = set1->vertexsuminList;
        vsum2 = set2->vertexsuminList;
        vsum = vsum1 + vsum2;                              //두 set의 vertex들의 총 개수
            
        esum1 = set1->edgesuminSet;
        esum2 = set2->edgesuminSet;
        esum = esum1 + esum2;                              //두 set의 edge weight의 총 합 (선택된 edge의 weight 총 합)
            
        
                       
        if(vsum1 == vsum2){                                //v1이 포함된 set과 v2가 포함된 set의 크기 같을 때
            
            if(v1->c_num <= v2->c_num){                    //v1의 도시 숫자가 더 작으면 v2가 흡수된다. v1->v2
                chk = 2;
                leaderV = set1->head;
                leaderV->second = set2->head->c_num;
                leaderV->secondSize = vsum2;
                
                
                cur = set2->head;
                while(cur != 0){
                    cur->htptr = set1;
                    cur = cur->next;
                }
                (set1->tail)->next = set2->head;
                set1->tail = set2->tail;
                
                delete set2;
                
               leaderV = set1->head;
                leaderV->htptr->vertexsuminList = vsum;
                leaderV->htptr->edgesuminSet = esum;
               
            }
            
          
            else{                                               //v2의 도시 숫자가 더 작으면 v1이 흡수된다. v2->v1
                chk = 1;
                cur = set1->head;
                
                leaderV = set2->head;
                leaderV->second = set1->head->c_num;
                leaderV->secondSize = vsum1;
                
                while(cur != 0){
                    cur->htptr = set2;
                    cur = cur->next;
                }
                (set2->tail)->next = set1->head;
                set2->tail = set1->tail;
                
                delete set1;
                
               leaderV = set2->head;
                leaderV->htptr->vertexsuminList = vsum;
                leaderV->htptr->edgesuminSet = esum;
                
            }
            
            
        }
        

        else if(vsum1 < vsum2){                                     //v1이 v2 에 흡수됨. v2->v1
            
            chk = 1;
            cur = set1->head;
            leaderV = set2->head;
            leaderV->second = set1->head->c_num;
            leaderV->secondSize = vsum1;
            
            while(cur != 0){
                cur->htptr = set2;
                cur = cur->next;
            }
            (set2->tail)->next = set1->head;
            set2->tail = set1->tail;
            
            delete set1;
            
           leaderV = set2->head;
            leaderV->htptr->vertexsuminList = vsum;
            leaderV->htptr->edgesuminSet = esum;
            
            
        }
        
        else{                                    // v1이 포함된 리스트 크기 > v2 가 포함된 리스트 크기 -> v2가 v1에 흡수됨. v1->v2
            chk = 2;
            cur = set2->head;
            
            leaderV = set1->head;
            leaderV->second = set2->head->c_num;
            leaderV->secondSize = vsum2;

            while(cur != 0){
                 cur->htptr = set1;
                 cur = cur->next;
             }
             (set1->tail)->next = set2->head;
             set1->tail = set2->tail;
             
             delete set2;
             
             leaderV = set1->head;
             leaderV->htptr->vertexsuminList = vsum;
             leaderV->htptr->edgesuminSet = esum;
            
            
             
        }
               
        return leaderV;
            
        }
    }
    
    
    
    Vertex* Kruskal(Edge *e){                                   //Iteration 을 한 번 수행하는 함수
        
         //Union 성공한것은 set이 갱신된 것이므로 set의 wieghtSum, set의 크기를 갱신해야한다.
        
        
        Vertex *leadV;
        
        Vertex *v1 = varr[e->city1_num];
        Vertex *v2 = varr[e->city2_num];
        
        leadV = Union(v1,v2);
        
        if(leadV != NULL){
            leadV->htptr->edgesuminSet+= e->distance;              //set의 edge의 weight 합 갱신
            
            
//            if(chk == 1){    //흡수된 set :  v1이 포함된 set
//                leadV->second = v1->htptr->head->c_num;
//                leadV->secondSize = v1->htptr->vertexsuminList;
//            }
//            else{           //흡수된 set :  v2가 포함된 set
//                leadV->second = v2->htptr->head->c_num;
//                leadV->secondSize = v2->htptr->vertexsuminList;
//            }
            
            
            
        }
        
        
        
   
        return leadV;
        
    }
    
    Vertex* cntVertex(Vertex* v, int k){        //도시 v가 포함된 set의 크기를 리턴한다.
        
        vector<Vertex*> vec;
        Vertex *res;
        
        Vertex *cur = v->htptr->head;
        
        if(v->htptr->vertexsuminList >=  k){      //v가 포함된 set의 크기가 k 보다 크거나 같음 -> k번째 도시의 번호, 이름 출력
            
            vec.push_back(cur);
            
            for(int i = 0; i<k; i++){
                cur = cur->next;
                vec.push_back(cur);
            }
            
            res = vec[k];
        }
        else{                       //v가 포함된 set의 크기가 k 보다 작음 -> no exist 출력
            res = NULL;
        }
        
        vec.clear();
        return res;
        
    }
    

    
//    Edge* betterEdge(vector<Edge> *vec){
//
//        vector<Edge> sameE;        //weight가 같은 edge를 저장하는 벡터
//        vector<Edge>::iterator iter=vec->begin();
//        vector<Edge>::iterator nextiter=iter;
//
//
//        int cnt=0;
//
//        for(iter = vec->begin(); iter != vec->end(); iter++){
//            nextiter=iter+1;
//
//
//            int d1 =iter[0].distance;
//            int d2 =nextiter[0].distance;
//
//            if(d1 == d2){
//                sameE.push_back(*iter);
//                cnt++;
//                continue;
//            }
//            else{
//                sameE.push_back(*iter);
//                cnt++;
//                break;
//            }
//        }
//
//        edgeCnt++;
//        Edge *selectedE = NULL;
//        for(int i= 0; i<cnt-1; i++){                     //sameE 벡터에는 edge weight가 같은 edge 객체들 있음
//
//            selectedE = &sameE[i];
//            int p1_1 = varr[selectedE[i].city1_num]->c_people;        //edge의 양쪽 vertex의 인구수
//            int p1_2 = varr[selectedE[i].city2_num]->c_people;
//            int p1 = p1_1 + p1_2;
//
//            int p2_1 = varr[sameE[i+1].city1_num]->c_people;
//            int p2_2 = varr[sameE[i+1].city2_num]->c_people;
//            int p2 = p2_1 + p2_2;
//
//
//            if(p1 > p2){
//                continue;
//            }
//            else if(p1 < p2) {   //seletecEdge 바뀜
//
//                selectedE = &sameE[i+1];
//            }
//            else{   //인구수의 합도 같은 경우
//                vector<int> min;
//                min.push_back(selectedE->city1_num);
//                min.push_back(selectedE->city2_num);
//                min.push_back(sameE[i+1].city1_num);
//                min.push_back(sameE[i+1].city2_num);
//
//                sort(min.begin(), min.end());
//
//                for(int j = 0; j<3; j++){
//                    if((min[0] == selectedE->city1_num) ||(min[0] == selectedE->city2_num)){
//                        continue;
//                    }
//                    else{
//                        selectedE = &sameE[j+1];
//                        break;
//                    }
//
//                }
//
//            }
//
//
//        }
//
//
//
//
//        return selectedE;
//
//    }
    
    
    
    
    
};






int main() {
 
    // clock_t start = clock();
    
    ofstream wFile;
    wFile.open("input.txt");        //파일을 연다. 파일 없으면 생성한다.
    
    cin >> N >> M >> q;
    wFile << N << " " << M << " " << q << endl;
 
    
    
    setList slt;
    
    
 
    for(int i = 0; i<N; i++){   // pvertex 개수만큼 도시번호,도시번호,도시인구를 입력받음.
        
        cin >> cityNum >> cityName >> cityPeople;
        slt.makeSet(cityNum, cityName, cityPeople);
        
     
        wFile << cityNum << " " << cityName << " " << cityPeople << endl;   //파일에 쓰기

    }

    
    for(int i = 0; i<M; i++){   //edge 개수만큼 도시번호1, 도시번호2, 도시간 도로 길이를 입력받음
        
        cin >> city1Num >> city2Num >> Distance;
        Edge E(city1Num, city2Num, Distance);
        edgeVec.push_back(E);
        
        wFile <<city1Num << " " << city2Num << " " << Distance << endl;    //파일에 쓰기
    }
    
    sort(edgeVec.begin(), edgeVec.end());     //edge의 distance를 기준으로 sorting
    
    for(int i = 0; i<edgeVec.size(); i++){
        cout << edgeVec[i].city1_num << " " << edgeVec[i].city2_num << " " << edgeVec[i].distance << endl;
    }
    
    //slt.betterEdge(&edgeVec);
    
    
    
    for(int i = 0; i<q; i++){       //질의 문을 q번 반복한다.
        
        char question;
        cin >> question;
        
        switch(question){
                
                
                
            case 'N': {          //도시 v가 포함된 집합에서 헤드로부터 k 번째 노들르 출력
                int v,k;        //도시번호, 정수
                cin >> v >> k;

                
                Vertex *res = slt.cntVertex(varr[v], k);
                
                if(res == NULL){
                    cout << "no exist" << endl;
                }
                else{
                    cout << res->c_num << " " << res->c_name << endl;
                }
                
                break;
             
                
            }
                
            case 'L' :{         //도시v가 포함된 linked-list의 총 element 의 개수에 대한 질의
                int v;
                cin >> v;       //도시번호
                
                cout << varr[v]->htptr->vertexsuminList << endl;
    
                break;
            }
                
                
                
                
                
            case 'I' :  {       //크루스칼 알고리즘에서 하나의 반복(iteration) 실행

                
                Vertex *res;
                
                
                    
                res = slt.Kruskal(&edgeVec[edgeCnt]);
                
        
                if(res == NULL){                            //Union 실패
                    cout << "not union" << endl;
                    edgeCnt++;
                    break;
                }
                else{
                    edgeF.push_back(edgeVec[edgeCnt]);
                    edgeFcnt++;
                    
                    if(edgeFcnt == N-1) {       //MST 완성
                        cout << res->second << " " <<res->secondSize << " " << res->c_num << " " << res->htptr->edgesuminSet << endl;
                        return 0;
                        
                    }
                    else{                       //MST 미완성
                        cout << res->c_num << " " << res->htptr->vertexsuminList << endl;
                        edgeCnt++;
                    }
                    
                
        
                }
            
                
                break;
                
                
            }
                
            case 'F': {          //두 도시가 같은 집합(set)에 속하는지 확인 -> ture or false
                int V1, V2;     //V1, V2는 도시번호
                cin >> V1 >> V2;
                
                if(slt.find(varr[V1])  ==  slt.find(varr[V2])){
                     cout << "True " << varr[V1]->htptr->head->c_num << endl;
                }
                else{
                    cout << "False " <<varr[V1]->htptr->head->c_num << " " << varr[V2]->htptr->head->c_num << endl;
                }
                
                break;
                
                
            }
                
                
            case 'W': { //도시 v를 포함하는 집합의 모든 포장도로들의 길이의 합
                int v;
                cin >> v;   //도시번호
                
                cout << varr[v]->htptr->edgesuminSet << endl;
                    
                
                break;
            }
            
                
            case 'Q' : {        //MST 완성할 때 까지 Kruskal 알고리즘 진행
                
                Vertex* res = NULL;
                while(edgeFcnt != N-1){
                    res = slt.Kruskal(&edgeVec[edgeCnt]);
                    if(res!=NULL){
                        edgeF.push_back(edgeVec[edgeCnt]);
                        edgeFcnt++;
                        
                    }
                    edgeCnt++;
                }
                
                //MST의 leader와 MST의 모든 포장도로들의 길이의 합 출력
                cout << res->c_num << " " << res->htptr->edgesuminSet << endl;
                return 0;
                break;
            }
                
                
            default:
                break;
                
                
                
                
        }
        
        
        
        
        
        
        
        
    }
    
    
    
    //clock_t end = clock();
    //cout << "time : " << ((double)(end-start)/CLOCKS_PER_SEC);




    return 0;
}
