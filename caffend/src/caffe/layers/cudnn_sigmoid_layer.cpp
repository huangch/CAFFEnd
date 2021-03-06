#ifdef USE_CUDNN
#include <vector>

#include "caffe/layers/cudnn_sigmoid_layer.hpp"

namespace caffe {

template <typename Dtype>
void CuDNNSigmoidLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  SigmoidLayer<Dtype>::LayerSetUp(bottom, top);
  // initialize cuDNN
  CUDNN_CHECK(cudnnCreate(&handle_));
  /* for ND support
  cudnn::createTensor4dDesc<Dtype>(&bottom_desc_);
  cudnn::createTensor4dDesc<Dtype>(&top_desc_);
  */
  cudnn::createTensorNdDesc<Dtype>(&bottom_desc_);
  cudnn::createTensorNdDesc<Dtype>(&top_desc_);
  /**/
  cudnn::createActivationDescriptor<Dtype>(&activ_desc_,
      CUDNN_ACTIVATION_SIGMOID);
  handles_setup_ = true;
}

template <typename Dtype>
void CuDNNSigmoidLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  SigmoidLayer<Dtype>::Reshape(bottom, top);
  /* for ND support
  const int N = bottom[0]->num();
  const int K = bottom[0]->channels();
  const int H = bottom[0]->height();
  const int W = bottom[0]->width();
  cudnn::setTensor4dDesc<Dtype>(&bottom_desc_, N, K, H, W);
  cudnn::setTensor4dDesc<Dtype>(&top_desc_, N, K, H, W);
  */
  cudnn::setTensorNdDesc<Dtype>(&bottom_desc_, bottom[0]->shape());
  cudnn::setTensorNdDesc<Dtype>(&top_desc_, top[0]->shape());
  /**/
}

template <typename Dtype>
CuDNNSigmoidLayer<Dtype>::~CuDNNSigmoidLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(this->bottom_desc_);
  cudnnDestroyTensorDescriptor(this->top_desc_);
  cudnnDestroy(this->handle_);
}

INSTANTIATE_CLASS(CuDNNSigmoidLayer);

}  // namespace caffe
#endif
