#ifndef PYKALDI2_DECODER_H_
#define PYKALDI2_DECODER_H_
#include <vector>
#include <memory>
#include "fst/fst-decl.h"
#include "base/kaldi-types.h"

// We have to do this hack with NO_KALDI_HEADERS, because Cython cannot compile
// with Kaldi headers, because of redefinition of unordered_map.
#ifndef NO_KALDI_HEADERS
#include "pykaldi2_decoder/pykaldi2_decoder_config.h"
#include "pykaldi2_decoder/pykaldi2_feature_pipeline.h"

#include "feat/online-feature.h"
#include "matrix/matrix-lib.h"
#include "util/common-utils.h"
#include "nnet2/online-nnet2-decodable.h"
#include "online2/online-endpoint.h"
#else
namespace kaldi{
    class PyKaldi2FeaturePipeline;
    class PyKaldi2DecoderImpl;

    class TransitionModel;
    namespace nnet2 {
      class DecodableNnet2Online;
      struct DecodableNnet2OnlineOptions;
      class AmNnet;
    }
    class LatticeFasterOnlineDecoder;
    struct LatticeFasterDecoderConfig;
    struct OnlineLatgenRecogniserConfig;
    template<typename > class VectorBase;
}

class PyKaldi2DecoderConfig;
#endif



namespace kaldi {

/// \addtogroup online_latgen 
/// @{

    class PyKaldi2Decoder {
    public:
        PyKaldi2Decoder(const string model_path);
        ~PyKaldi2Decoder();

        int32 Decode(int32 max_frames);
        void FrameIn(unsigned char *frame, int32 frame_len);
        void FrameIn(VectorBase<BaseFloat> *waveform_in);
        bool GetBestPath(std::vector<int> *v_out, BaseFloat *prob);
        bool GetLattice(fst::VectorFst<fst::LogArc> * out_fst, double *tot_lik, bool end_of_utt=true);
        string GetWord(int word_id);
        void InputFinished();
        bool EndpointDetected();
        void FinalizeDecoding();
        void Reset();
    private:
        PyKaldi2FeaturePipeline *feature_pipeline_;

        fst::StdFst *hclg_;
        LatticeFasterOnlineDecoder *decoder_;
        TransitionModel *trans_model_;
        nnet2::AmNnet *am_;
        fst::SymbolTable *words_;
        PyKaldi2DecoderConfig *config_;
        nnet2::DecodableNnet2Online *decodable_;

        void InitTransformMatrices();
        void LoadDecoder();
        void ParseConfig();
        void Deallocate();
    };

/// @} end of "addtogroup online_latgen"

} // namespace kaldi

#endif  // #ifdef PYKALDI2_DECODER_H_