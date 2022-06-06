#include <iostream>
#include <fstream>
#include <stack>
#include <random>
#include <string>


//Forward Declarations for elements in the data structure being visited. This will include SmallLang language elemnts in an Abstract Syntax Tree (AST) for your assignment.
class PointConn;
class TriConn;
class QuadConn;
class PentaConn;

//Visitor abstract class (Interface). All visitor implementations (extensions to this class) have to handle Triangle, Rectangle and Quad.
class Visitor
{
  public:
    virtual void visit(PointConn*     po) = 0;
    virtual void visit(TriConn*       tr) = 0;
    virtual void visit(QuadConn*      qu) = 0;
    virtual void visit(PentaConn*     pe) = 0;
};

//Some state we'll pass along whilst traversing the data structure. The symbol table in the case of your assignment.
class VisitorStateNodeCount
{
    int leafcount;
    int edgecount;
    int triscount;
    int quadcount;
    int pentacount; 

    public:
      VisitorStateNodeCount() 
      : leafcount(0), edgecount(0), triscount(0), quadcount(0), pentacount(0) 
      { }

      void IncLeafCount()
      {
        leafcount++;
      }

      void IncTrisCount()
      {
        triscount++;
      }

      void IncQuadCount()
      {
        quadcount++;
      }

      void IncPentaCount()
      {
        pentacount++;
      }

      void IncEdgeCount()
      {
        edgecount++;
      }

      int GetLeafCount() { return leafcount; }
      int GetTrisCount() { return triscount; }
      int GetQuadCount() { return quadcount; }
      int GetPentaCount() { return pentacount; }
      int GetEdgeCount() { return edgecount; }

};

//Some state we'll pass along whilst traversing the data structure. The symbol table in the case of your assignment.
class VisitorStateGraphDraw
{
    public:
      std::ofstream outfile;

      VisitorStateGraphDraw()  
      { 
         outfile.open("graph.dot");
         outfile << "digraph randompandemic {" << std::endl;
      }
};

//Visitable elements interface. The is the parent of all elements that can be visited by visitor classes.
//-----------------
class Connector
{
  int child_count;
  int child_conn;
  std::string label;

  public:

    virtual void AddChild(Connector * p_child) = 0;    //Used to autoatically build the structure for this example
    
    void SetChildConn(int p_act) { child_conn = p_act; }
    void IncChildConn() { child_conn++; }
    int  GetChildConn() { return child_conn; }
    int  GetChildCount() { return child_count; }
    void SetChildCount(int p_cc) { child_count = p_cc; }
    void SetLabel(std::string p_label) {label = p_label; }
    std::string GetLabel() {return label; }

    //Visitor handle
    virtual void accept(Visitor* v) = 0;  

};
//-----------------

//Concrete Visitable classes extend Shape and override the accept function.
//-----------------
class PointConn : public Connector
{
  int v;

  public:

    PointConn(int p_v) : v(p_v) { SetChildCount(0); SetChildConn(0); }

    virtual void AddChild(Connector * p_child) override
    {
    }

    virtual void accept(Visitor* v) override
    {
      v->visit(this);
    }
};

class TriConn : public Connector
{  
  public:
    Connector* c[3];

    TriConn() { SetChildCount(3); SetChildConn(0); }

    TriConn(Connector * p_c0, Connector * p_c1, Connector *p_c2) 
      { 
        SetChildren(p_c0, p_c1, p_c2);
        SetChildCount(3); 
        SetChildConn(3); 
      }

    void SetChildren(Connector * p_c0, Connector * p_c1, Connector *p_c2) 
    {
      c[0] = p_c0; c[1] = p_c1; c[2] = p_c2;
      SetChildConn(3);
    }

    virtual void AddChild(Connector * p_child) override
    {
      c[GetChildConn()] = p_child;
      IncChildConn();
    }

    virtual void accept(Visitor* v) override
    {
      v->visit(this);
    }
};

class QuadConn : public Connector
{
  public:
    Connector* c[4];

    QuadConn() { SetChildCount(4); SetChildConn(0); }

    QuadConn(Connector * p_c0, Connector * p_c1, Connector *p_c2, Connector* p_c3) 
      { 
        SetChildren(p_c0, p_c1, p_c2, p_c3);
        SetChildCount(4); 
        SetChildConn(4); 
      }
    
    void SetChildren(Connector * p_c0, Connector * p_c1, Connector *p_c2, Connector *p_c3)
    {
      c[0] = p_c0; c[1] = p_c1; c[2] = p_c2; c[3] = p_c3;
      IncChildConn();
    }

    virtual void AddChild(Connector * p_child) override
    {
      c[GetChildConn()] = p_child;
      IncChildConn();
    }

    virtual void accept(Visitor* v) override
    {
      v->visit(this);
    }
};

class PentaConn : public Connector
{
  public:
    Connector *c[5];

    PentaConn() { SetChildCount(5); SetChildConn(0); }

    PentaConn(Connector * p_c0, Connector * p_c1, Connector *p_c2, Connector* p_c3, Connector* p_c4) 
    {
      SetChildren(p_c0, p_c1, p_c2, p_c3, p_c4);
      SetChildCount(5);
      SetChildConn(5);
    }

    void SetChildren(Connector * p_c0, Connector * p_c1, Connector *p_c2, Connector *p_c3, Connector *p_c4) 
    {
      c[0] = p_c0; c[1] = p_c1; c[2] = p_c2; c[3] = p_c3; c[4] = p_c4;
      IncChildConn();
    }

    virtual void AddChild(Connector * p_child) override
    {
      c[GetChildConn()] = p_child;
      IncChildConn();
    }

    virtual void accept(Visitor* v) override
    {
      v->visit(this);
    }
};
//------------------


//A concrete implementation of Visitor which simply counts the number of edges of the shapes in the data structure visited .
class NodeCountVisitor : public Visitor
{
  VisitorStateNodeCount vs;  

  public:
    virtual void visit(PointConn * t) override
    {
      vs.IncLeafCount();
    }

    virtual void visit(TriConn * t) override
    { 
      vs.IncTrisCount();
      t->c[0]->accept(this);
      t->c[1]->accept(this);
      t->c[2]->accept(this);
    }

    virtual void visit(QuadConn * r) override
    {
      vs.IncQuadCount();
      r->c[0]->accept(this);
      r->c[1]->accept(this);
      r->c[2]->accept(this);
      r->c[3]->accept(this);
    }

    virtual void visit(PentaConn * q) override
    {
      vs.IncPentaCount();
      q->c[0]->accept(this);
      q->c[1]->accept(this);
      q->c[2]->accept(this);
      q->c[3]->accept(this);
      q->c[4]->accept(this);
    }

    int GetLeafCount()
    {
      return vs.GetLeafCount();
    }

    int GetTrisCount()
    {
      return vs.GetTrisCount();
    }

    int GetQuadCount()
    {
      return vs.GetQuadCount();
    }

    int GetPentaCount()
    {
      return vs.GetPentaCount();
    }
};

class GraphVizGeneratorVisitor : public Visitor
{
  VisitorStateGraphDraw vs;  

  public:
    virtual void visit(PointConn * t) override
    {
       if (t->GetLabel() == "ROOT") { vs.outfile << t->GetLabel() << std::endl << " }" << std::endl; vs.outfile.close(); }
    }

    virtual void visit(TriConn * t) override
    { 
      vs.outfile << t->GetLabel() << "->" << t->c[0]->GetLabel() << std::endl;
      vs.outfile << t->GetLabel() << "->" << t->c[1]->GetLabel() << std::endl;
      vs.outfile << t->GetLabel() << "->" << t->c[2]->GetLabel() << std::endl;

      t->c[0]->accept(this);
      t->c[1]->accept(this);
      t->c[2]->accept(this);

      if (t->GetLabel() == "ROOT") { vs.outfile << "}" << std::endl; vs.outfile.close(); }
    }

    virtual void visit(QuadConn * q) override
    {
      vs.outfile << q->GetLabel() << "->" << q->c[0]->GetLabel() << std::endl;
      vs.outfile << q->GetLabel() << "->" << q->c[1]->GetLabel() << std::endl;
      vs.outfile << q->GetLabel() << "->" << q->c[2]->GetLabel() << std::endl;
      vs.outfile << q->GetLabel() << "->" << q->c[3]->GetLabel() << std::endl;

      q->c[0]->accept(this);
      q->c[1]->accept(this);
      q->c[2]->accept(this);
      q->c[3]->accept(this);

      if (q->GetLabel() == "ROOT") { vs.outfile << "}" << std::endl; vs.outfile.close(); }
    }

    virtual void visit(PentaConn * p) override
    {
      vs.outfile << p->GetLabel() << "->" << p->c[0]->GetLabel() << std::endl;
      vs.outfile << p->GetLabel() << "->" << p->c[1]->GetLabel() << std::endl;
      vs.outfile << p->GetLabel() << "->" << p->c[2]->GetLabel() << std::endl;
      vs.outfile << p->GetLabel() << "->" << p->c[3]->GetLabel() << std::endl;
      vs.outfile << p->GetLabel() << "->" << p->c[4]->GetLabel() << std::endl;

      p->c[0]->accept(this);
      p->c[1]->accept(this);
      p->c[2]->accept(this);
      p->c[3]->accept(this);
      p->c[4]->accept(this);

      if (p->GetLabel() == "ROOT") { vs.outfile << "}" << std::endl; vs.outfile.close(); }
    }
};

//-----------------------

//In your assignment, your parser would generate the structure which will be visited by the visitor classes.
class StructureGenerator     
{
  //Probabilities of randomly generating a shape. Values = 1 - prob.
  float prob_point;
  float prob_tri;
  float prob_quad;
  float prob_penta;

  int max_nodes = 1000;     //Beyond this number all new connections are PointConn !!
  int node_count = 0;

  Connector * root_node;

  std::stack<Connector*> stekk;

  public:
    StructureGenerator(float p_ppo = 0.1f, float p_ptri = 0.7f, float p_quad = 0.8f, float p_pen = 0.95f)
      : prob_point(p_ppo), prob_tri(p_ptri), prob_quad(p_quad), prob_penta(p_pen)
    { 
        
    }

    Connector * GetRandomConnector()
    {
      std::random_device rd;  //Will be used to obtain a seed for the random number engine
      std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
      std::uniform_real_distribution<> dis(0.0, 1.0);

      node_count++;

      if (node_count < max_nodes)
      {
        float roll = dis(gen);

        if (roll > prob_penta) return new PentaConn();
        if (roll > prob_quad)  return new QuadConn();
        if (roll > prob_tri)   return new TriConn();
      }

      return new PointConn(0);     //Highest probability of returing a leaf of the graph - someone in self-quarantine and hopefully stop the pandemic!!
    }

    void BuildStructure()
    {
        root_node = GetRandomConnector();   // When this is a PointConn the graph will only have one node. Root is randomly chosen.
        root_node->SetLabel("ROOT");      
        stekk.push(root_node);
        while (!stekk.empty())
        {
            Connector * top = stekk.top(); stekk.pop();
            int childCount = top->GetChildCount();
            for (int i = 0; i<childCount; i++)
            {
               Connector * ns = GetRandomConnector();
               ns->SetLabel("N"+std::to_string(node_count));
               top->AddChild(ns);
               stekk.push(ns);
            }
        }
    }

    Connector * GetRootNode()
    {
       return root_node;
    }
};

//Quick test ...
int main(int argc, char* argv[])
{
    std::cout << "A simple Visitor Design Pattern example !!" << std::endl;
    
    NodeCountVisitor * ncv = new NodeCountVisitor();
    GraphVizGeneratorVisitor * gvv = new GraphVizGeneratorVisitor();

    //A simple example
    //Connector * p1 = new Point(1);
    //Connector * p2 = new Point(2);
    //Connector * p3 = new Point(3);
    //Connector * p4 = new Point(4);
    //Connector * p5 = new Point(5);
    //Connector * t1 = new Triangle(p1, p2, p3);
    //Connector * t2 = new Triangle(t1, p4, p5);  //this is the root node
    //t2->accept(ncv);
    //t2->accept(gvv);

    //==============//
    //Generate Random Hierarchies of node types (tri, quad, penta and point)
    StructureGenerator sg;
    sg.BuildStructure();
    

    //==============// 
    //Use Node Count Visitor to determine counts of the different node types
    sg.GetRootNode()->accept(ncv);
    std::cout << "Number of penta  =  " << ncv->GetPentaCount() << std::endl;
    std::cout << "Number of quads  =  " << ncv->GetQuadCount() << std::endl;
    std::cout << "Number of tris   =  " << ncv->GetTrisCount() << std::endl;
    std::cout << "Number of points =  " << ncv->GetLeafCount() << std::endl;
     
    //==============//
    //Use GraphViz visitor to generate a GraphViz graph description file. From the command line, use the dot utility in GraphViz to export to a PDF.
    sg.GetRootNode()->accept(gvv);



    //End of demonstration program demonstrating how the Visitor design pattern is used. You will be using it to implemet tasks 3,4 and 5.

}