package blockchain;

@license(LGPL);


import ned.IdealChannel;

network Blockchain
{
    parameters:
        int n = default(5);
        string peers = default("");
    submodules:
        nodes[n]: Node ;
    connections: 
       for i=0..sizeof(nodes)-1, for j=i+1..sizeof(nodes)-1
       {
           nodes[i].outs++ --> IdealChannel --> nodes[j].ins++;
           nodes[i].ins++ <-- IdealChannel <-- nodes[j].outs++;
       }  
}
