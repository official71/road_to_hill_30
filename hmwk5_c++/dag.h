#ifndef _DAG_H
#define _DAG_H

#include <string>
#include <utility>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "basic.h"
#include "dfs.h"

using namespace std;

struct directed_acyclic_graph 
{
    base_graph bg;

    directed_acyclic_graph() {}
    directed_acyclic_graph(base_graph& base);
    directed_acyclic_graph(vector<Vertex_ptr>& vv);
    directed_acyclic_graph(vector<Edge_ptr>& ve);
    directed_acyclic_graph(vector<Vertex_ptr>& vv, vector<Edge_ptr>& ve);

    void copy_base(base_graph& base, bool trusted=false);

    base_graph& base() { return bg; }
    Graph_vertices& vertices() { return bg.vertices; }
    Graph_edges& edges_from() { return bg.edges_from; }
    Graph_edges& edges_to() { return bg.edges_to; }

    bool has_vertex(Vertex_ptr v) { return bg.vertex_in_graph(v); }
    bool has_edge(Edge_ptr e) { return bg.edge_in_graph(e); }

    Val value(Vertex_ptr v) { return bg.vertex_value(v); }
    Val value(Edge_ptr e) { return bg.edge_value(e); }
    void set_value(Vertex_ptr v, Val val) { bg.set_vertex_value(v, val); }
    void set_value(Edge_ptr e, Val val) { bg.set_edge_value(e, val); }

    void vertices_insert(Vertex_ptr v) { bg.vertices_insert(v); }
    void vertices_erase(Vertex_ptr v) { bg.vertices_erase(v); }

    void edges_from_insert(Vertex_ptr f, Edge_ptr e) { bg.edges_from_insert(f, e); }
    void edges_to_insert(Vertex_ptr t, Edge_ptr e) { bg.edges_to_insert(t, e); }

    Edge_ptr edges_from_find(Vertex_ptr f, Vertex_ptr t) { return bg.edges_from_find(f, t); }
    Edge_ptr edges_to_find(Vertex_ptr f, Vertex_ptr t) { return bg.edges_to_find(f, t); }
    Edge_ptr edges_find(Vertex_ptr f, Vertex_ptr t) { return bg.edges_from_find(f, t); }

    Graph_vertices edges_from_neighbors(Vertex_ptr f) { return bg.edges_from_neighbors(f); }
    Graph_vertices edges_to_neighbors(Vertex_ptr t) { return bg.edges_to_neighbors(t); }

    void edges_from_erase(Vertex_ptr f, Edge_ptr e) { bg.edges_from_erase(f, e); }
    void edges_to_erase(Vertex_ptr t, Edge_ptr e) { bg.edges_to_erase(t, e); }
    void edges_erase(Vertex_ptr f, Vertex_ptr t) { bg.edges_from_to_erase(f, t); }

    /* what makes it special */
    bool is_cyclic()
    {
        depth_first_search DFS;
        return DFS.is_cyclic(bg);
    }

    /* what makes it special */
    vector<Vertex_ptr> topological_order();

    string to_string() 
    {
        return "\nTYPE: Directed Acyclic Graph" \
         "\nVERTICES:\n" + bg.vertices_to_string() + \
         "\nEDGES:\n" + bg.edges_to_string(edges_from());
    }
};

directed_acyclic_graph::directed_acyclic_graph(vector<Vertex_ptr>& vv)
{
    /* no need to check cyclic since there is no edges */
    bg = base_graph(vv);
}

directed_acyclic_graph::directed_acyclic_graph(vector<Edge_ptr>& ve)
{
    base_graph tmp_bg = base_graph(ve);
    depth_first_search DFS;
    if (!DFS.is_cyclic(tmp_bg))
        copy_base(tmp_bg, true);
    else
        _DEBUG("ERROR: DAG not constructed due to cyclic input.");
}

directed_acyclic_graph::directed_acyclic_graph(vector<Vertex_ptr>& vv, vector<Edge_ptr>& ve)
{
    base_graph tmp_bg = base_graph(vv);
    tmp_bg.edges_vector_insert(ve);
    depth_first_search DFS;
    if (!DFS.is_cyclic(tmp_bg))
        copy_base(tmp_bg, true);
    else
        _DEBUG("ERROR: DAG not constructed due to cyclic input.");
}

directed_acyclic_graph::directed_acyclic_graph(base_graph& base)
{
    depth_first_search DFS;
    if (!DFS.is_cyclic(base))
        copy_base(base, true);
    else
        _DEBUG("ERROR: DAG not constructed due to cyclic input.");
}

void directed_acyclic_graph::copy_base(base_graph& base, bool trusted)
{
    bool cyclic = false;
    if (!trusted) {
        depth_first_search DFS;
        cyclic = DFS.is_cyclic(base);
    }
    if (!cyclic)
        bg = base;
    else
        _DEBUG("ERROR: DAG not updated due to cyclic input.");
}

vector<Vertex_ptr> directed_acyclic_graph::topological_order()
{
    depth_first_search DFS;
    return DFS.dfs(bg);
}

#endif