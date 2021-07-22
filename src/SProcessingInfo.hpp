#pragma once

struct SProcessingInfo{
    enum EGrayscale{
        BROAD,
        SIMPLE
    };
    EGrayscale grayscale_ = SProcessingInfo::EGrayscale::BROAD;
};
