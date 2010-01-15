#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include "Node.h"
#include "CoreUtilities/ClassFactory.h"
#include "win32dll.h"
#include <string>

using namespace std;

class DLLIMPORTEXPORT CompParameter
{
public:
    CompParameter(const string name, const string description, const string unit, double &rValue); //should maybe be a description field as well
    ///TODO: getting strings can (probably) be speed up by returning const references instead of copying strings
    string getName();
    string getDesc();
    string getUnit();
    double getValue();
    void setValue(const double value);

private:
    string mName;
    string mDescription;
    string mUnit;
    double* mpValue;
};


class Component; //forward declaration
class ComponentSystem;  //forward declaration

class DLLIMPORTEXPORT Port ///TODO: Should be made virtual somehow OR Change Port into PowerPort
{
    friend class Component;
    friend class ComponentSystem;

public:
    Port();
    Port(string portname, string node_type);
    string &getNodeType(); ///TODO: Move to protected
    Node &getNode(); ///TODO: Move to protected
    Node *getNodePtr(); ///TODO: Move to protected
    virtual double ReadNode(const size_t idx);
    virtual void WriteNode(const size_t idx, const double value);
    bool isConnected();

protected:
    string mPortType;
    void setNode(Node* node_ptr);

private:
    string mPortName, mNodeType;
    Node* mpNode;
    Component* mpComponent;
    bool mIsConnected;
};

class DLLIMPORTEXPORT PowerPort :public Port
{
public:
    PowerPort();
    PowerPort(string portname, string node_type);
};


class DLLIMPORTEXPORT ReadPort :public Port
{
public:
    ReadPort();
    ReadPort(string portname, string node_type);

    void WriteNode(const size_t idx, const double value);
};


class DLLIMPORTEXPORT WritePort :public Port
{
public:
    WritePort();
    WritePort(string portname, string node_type);

    double ReadNode(const size_t idx);
};


class DLLIMPORTEXPORT Component
{
    friend class ComponentSystem;

public:
    Component(string name, double timestep=0.001);
    virtual ~Component(){};
    virtual void initialize(); ///TODO: Default values are hard set
    virtual void simulateOneTimestep();
    void simulate(const double startT, const double Ts);

    void setName(string name);
    string &getName();

    string &getType();

    void registerParameter(const string name, const string description, const string unit, double &rValue);
    void listParametersConsole();
    double getParameter(const string name);
    void setParameter(const string name, const double value);

    void setTimestep(const double timestep);
    double getTimestep();

    bool isComponentC();
    bool isComponentQ();
    bool isComponentSystem();
    bool isComponentSignal();

    void setSystemparent(ComponentSystem &rComponentSystem); ///TODO: this should not be public
    ComponentSystem &getSystemparent();
    Port &getPortById(const size_t port_idx); ///TODO: this should not be public
    Port &getPort(const string portname);
    bool getPort(const string portname, Port &rPort);

protected:
    //void addPort(const size_t port_idx, Port port);
    void addPort(const string portname, const string nodetype, const int id=-1); ///TODO: Should be deleted
    void addPowerPort(const string portname, const string nodetype, const int id=-1); ///TODO: Implement nicer, very small difference between addPort, addReadPort and addWritePort
    void addReadPort(const string portname, const string nodetype, const int id=-1); ///TODO: Implement nicer, very small difference between addPort, addReadPort and addWritePort
    void addWritePort(const string portname, const string nodetype, const int id=-1); ///TODO: Implement nicer, very small difference between addPort, addReadPort and addWritePort
    //void addMultiPort(const string portname, const string nodetype, const size_t nports, const size_t startctr=0);

    void addInnerPortSetNode(const string portname, Node &rNode);

    void addSubNode(Node* node_ptr);


    string mType;
    vector<Port*> mPortPtrs, mInnerPortPtrs;

    vector<Node*> mSubNodePtrs;

    vector<CompParameter> mParameters;

    double mTimestep;
    double mTime;

    bool mIsComponentC;
    bool mIsComponentQ;
    bool mIsComponentSystem;
    bool mIsComponentSignal;

private:
    string mName;
    ComponentSystem* mpSystemparent;
};


class DLLIMPORTEXPORT ComponentSignal :public Component
{
public:
    ComponentSignal(string name, double timestep=0.001);
};


class DLLIMPORTEXPORT ComponentC :public Component
{
public:
    ComponentC(string name, double timestep=0.001);
};


class DLLIMPORTEXPORT ComponentQ :public Component
{
public:
    ComponentQ(string name, double timestep=0.001);
};


class DLLIMPORTEXPORT ComponentSystem :public Component
{
    public:
    ComponentSystem(string name, double timestep=0.001);
    void addComponents(vector<Component*> components);
    void addComponent(Component &rComponent);
    void addInnerPortSetNode(const string portname, Node &rNode);
    void preAllocateLogSpace(const double startT, const double stopT);
    void logAllNodes(const double time);
    void connect(Component &rComponent1, const string portname1, Component &rComponent2, const string portname2);
    void simulate(const double startT, const double stopT);

    protected:
    vector<Component*> mSubComponentPtrs; //Problems with inheritance and casting?
    vector<Component*> mComponentSignalptrs;
    vector<Component*> mComponentQptrs;
    vector<Component*> mComponentCptrs;

    NodeFactory mpNodeFactory;

    private:
};

typedef ClassFactory<string, Component> ComponentFactory;
extern ComponentFactory gCoreComponentFactory;
DLLIMPORTEXPORT ComponentFactory* getCoreComponentFactoryPtr();

#endif // COMPONENT_H_INCLUDED
