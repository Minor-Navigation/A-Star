#include <bits/stdc++.h>
using namespace std;

#define min(a,b) ((a<b) ? (a) : (b))
#define max(a,b) ((a>b) ? (a) : (b))
#define abs(a) ((a>0) ? (a) : ((-1)*a))
#define ll long long
#define FOR(i,n) for(int i=0;i<n;i++)
#define FORL(i,n) for(long long i=0;i<n;i++)
#define MOD 1000000007
#define PI 3.141592653589
#define pb push_back
#define mp make_pair
int dp[5009][5009]={0};
#define fastio ios_base::sync_with_stdio(false); cin.tie(0);
#define fileio freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
#define MAX_CHILD 5
#define null NULL
#define square(a) a*a

class nodeInd{
public:
    ll id,ptr,adjPtr;
    int size;
};

class nodeData {
public:
    ll id;
    double lon,lat;
    vector<ll> adj,adjPtr;
    char* otherData;
} ;
nodeData queryNode(ll id)
{

    fstream f,f1,f2;
    nodeData nd;
    f.open("NodeIndexNew.ind",ios::in | ios::out | ios::binary); 
    f1.open("CompleteNodeData.dat",ios::in | ios::out | ios::binary); 
    f2.open("adjacency.dat",ios::in | ios::out | ios::binary); 
    char *res = NULL;

    f.seekg(0,ios::end);
    ll no_nodes=f.tellg();
    nodeInd in;
    no_nodes/=sizeof(nodeInd);
    int size;
    ll temp;
    ll s=0,e=no_nodes-1,m;
    while(s<=e)
    {
        m=(s+e)/2;
        f.seekg(m*sizeof(nodeInd));f.seekp(m*sizeof(nodeInd));
        f.read((char*)&in,sizeof(nodeInd));
        if(in.id==id)
        {
            //cout<<"Found";
            nd.otherData= new char[in.size];
            f1.seekg(in.ptr);f1.seekp(in.ptr);
            f1.read(nd.otherData,in.size);

            f2.seekg(in.adjPtr);f2.seekp(in.adjPtr);
            f2>>nd.id>>nd.lon>>nd.lat>>size;
            while(size--)
            {
                f2>>temp;
                nd.adj.pb(temp);
                f2>>temp;
                nd.adjPtr.pb(temp);
            }

            return nd;
        }
        if(in.id<id)
            s=m+1;
        else
            e=m-1;

    }
    nd.id=-1;
    
    return nd;

}


nodeData queryNode(ll id, ll ptr)
{

    fstream f,f1,f2;
    nodeData nd;
    f.open("NodeIndexNew.ind",ios::in | ios::out | ios::binary); 
    f1.open("CompleteNodeData.dat",ios::in | ios::out | ios::binary); 
    f2.open("adjacency.dat",ios::in | ios::out | ios::binary); 
    char *res = NULL;

    nodeInd in;
    int size;
    ll temp;

   
    f.seekg(ptr);f.seekp(ptr);
    f.read((char*)&in,sizeof(nodeInd));
    if(in.id==id)
    {
        //cout<<"Found";
        nd.otherData= new char[in.size];
        f1.seekg(in.ptr);f1.seekp(in.ptr);
        f1.read(nd.otherData,in.size);

        f2.seekg(in.adjPtr);f2.seekp(in.adjPtr);
        f2>>nd.id>>nd.lon>>nd.lat>>size;
        while(size--)
        {
            f2>>temp;
            nd.adj.pb(temp);
            f2>>temp;
            nd.adjPtr.pb(temp);
        }

        return nd;
    }
    nd.id=-1;
    
    return nd;

}

#define earthRadiusKm 6371.0
double deg2rad(double deg) {
  return (deg * PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / PI);
}

class Astar_node{
public:
    nodeData nd;
    double travelDist, remainingDist;
    double distFrom(double lon,double lat)
    {
        //nd.lon  , nd.lat

        double lat1r, lon1r, lat2r, lon2r, u, v;
        lat1r = deg2rad(lat);
        lon1r = deg2rad(lon);
        lat2r = deg2rad(nd.lat);
        lon2r = deg2rad(nd.lon);
        u = sin((lat2r - lat1r)/2.0);
        v = sin((lon2r - lon1r)/2.0);
        return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
    }

    bool operator< (const Astar_node &a)
    {
        return ((travelDist+remainingDist) > (a.travelDist+a.remainingDist));
    }
    
};

bool operator< (const Astar_node &a , const Astar_node &b)
{
    return ((b.travelDist+b.remainingDist) < (a.travelDist+a.remainingDist));
}
vector<ll> AStar(ll source, ll destination)
{
    
    map<ll,ll> parent;
    map<ll,bool> visited;
    vector<ll> path;
    Astar_node src,cur,child;
    nodeData dest;
    src.nd=queryNode(source);
    dest=queryNode(destination);
    if(src.nd.id ==-1 || dest.id ==-1)
        return path;
    parent[src.nd.id]=-1;
    src.travelDist = 0.0;
    src.remainingDist = src.distFrom(dest.lon,dest.lat);

    //cout<<src.nd.id<<" "<<parent[src.nd.id]<<" "<<src.travelDist<< " " <<src.remainingDist<<endl;
    
    priority_queue <Astar_node> q;
    q.push(src);
    int i;

    while(!q.empty())
    {
        cur=q.top();
        q.pop();
        visited[cur.nd.id]=true;
        for(i=0;i<cur.nd.adj.size();i++)
        {
            child.nd = queryNode(cur.nd.adj[i],cur.nd.adjPtr[i]);
            if(child.nd.id ==-1)
                continue;
            if(visited.find(child.nd.id) != visited.end())
                continue;
            visited[child.nd.id]=true;
            child.travelDist = cur.travelDist + child.distFrom(cur.nd.lon,cur.nd.lat);
            child.remainingDist = child.distFrom(dest.lon,dest.lat);
             //cout<<child.nd.id<<" "<<cur.nd.id<<" "<<child.travelDist<< " " <<child.remainingDist<<"\t";
             parent[child.nd.id]=cur.nd.id;
            if(child.remainingDist == 0.0) break;
            

            q.push(child);

           




        }
        //cout<<endl;
        if(child.remainingDist==0.0) break;

        

    }

    
    path.pb(dest.id);
    ll it = parent[dest.id];
    while(it!=-1)
    {
        path.pb(it);
        it=parent[it];
    }

    reverse(path.begin(), path.end());

   
    return path;


}

int main()
{
    
    //freopen("input.txt", "r", stdin); 
    freopen("output.txt", "w", stdout);


    //2001957780 s
    //2001957719 d

    vector<ll> path = AStar(910664803,913535624);
    
    for (int i=0;i<path.size();i++)
    {
        cout<<path[i]<<endl;
    }


 //    ll id,ptr;
 //    char s1[27];
 //    int size;
    // nodeInd in;
    
    
 //    int t=10;
 //    while(!f.eof())
 //    {
 //     if(f.read(s1,27)){
 //         cout<<s1<<endl;

 //     }       
 //    }
 //    f.close();
 //    freopen("data.txt", "r", stdin); 
 //    freopen("output.txt", "w", stdout);
 //    f.open("NodeIndexNew.ind",ios::in | ios::out | ios::trunc);  
 //    cout<<"temp data done"<<endl;
 //    while(cin>>id>>ptr>>size)
 //    {
 //     in.id=id;
 //     in.ptr=ptr;
 //     in.size=size;
 //     f.write((char*)&in,sizeof(in));

 //    }



    return 0;
}
