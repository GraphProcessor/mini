#pragma once

#include "problem.hxx"

namespace gunrock {
namespace bfs {

struct bfs_problem_t : problem_t {
  mem_t<int> d_labels;
  mem_t<int> d_preds;
  std::vector<int> labels;
  std::vector<int> preds;
  int src;

  struct data_slice_t {
      int *d_labels;
      int *d_preds;

      void init(mem_t<int> &_labels, mem_t<int> &_preds) {
        d_labels = _labels.data();
        d_preds = _preds.data();
      }
  };

  mem_t<data_slice_t> d_data_slice;
  std::vector<data_slice_t> data_slice;
  
  bfs_problem_t() {}

  bfs_problem_t(const bfs_problem_t& rhs) = delete;
  bfs_problem_t& operator=(const bfs_problem_t& rhs) = delete;

  bfs_problem_t(std::shared_ptr<graph_device_t> rhs, size_t src, standard_context_t& context) :
      problem_t(rhs),
      src(src),
      data_slice( std::vector<data_slice_t>(1) ) {
          labels = std::vector<int>(rhs->num_nodes, -1);
          preds = std::vector<int>(rhs->num_nodes, -1);
          labels[src] = 0;
          preds[src] = -1;
          d_labels = to_mem(labels, context);
          d_preds = to_mem(preds, context);
          data_slice[0].init(d_labels, d_preds);
          d_data_slice = to_mem(data_slice, context);
      }

  void extract() {
      mgpu::dtoh(labels, d_labels.data(), gslice->num_nodes); 
  }
};

} //end bfs
} //end gunrock
