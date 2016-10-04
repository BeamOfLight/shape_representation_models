/**
  https://github.com/BeamOfLight/shape_representation_models.git
  abstract_other_model.h

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#pragma once
#ifndef SHAPE_REPRESENTATION_MODELS_ABSTRACT_OTHER_MODEL_H
#define SHAPE_REPRESENTATION_MODELS_ABSTRACT_OTHER_MODEL_H

#include <shape_representation_models/abstract_model.h>

namespace ShapeRepresentationModels {
  class AbstractOtherModel: public AbstractModel
  {
    public:
    	int getMethodType() const;
  };
}

#endif
