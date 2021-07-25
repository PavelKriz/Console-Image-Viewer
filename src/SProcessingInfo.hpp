#pragma once

struct SProcessingInfo{
    enum EGrayscale{
        BROAD,
        SIMPLE
    };
    bool histogramEqualisation_;
    EGrayscale grayscale_ = SProcessingInfo::EGrayscale::BROAD;
};
