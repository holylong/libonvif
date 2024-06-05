add_library(lottie SHARED
        lottieanimation.cpp lottieanimation.h
        rasterrenderer/batchrenderer.cpp rasterrenderer/batchrenderer.h
        rasterrenderer/lottierasterrenderer.cpp rasterrenderer/lottierasterrenderer.h)

target_link_libraries(lottie lottiep)