/**
  https://github.com/BeamOfLight/shape_representation_models.git
  configuration.h

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_CONFIGURATION_H
#define SHAPE_REPRESENTATION_MODELS_CONFIGURATION_H

namespace ShapeRepresentationModels {
  struct Configuration
  {
    //int firstImageId;
    //int lastImageId;
    int showImagesFlag;
    int showNotZeroDiffCounter;
    int threadsCount;
    int showImageIds;
    bool enableMultithreading;
    std::vector < std::string > modelNames;
  };
}

#endif
