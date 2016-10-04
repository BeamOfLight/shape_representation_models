/**
  https://github.com/BeamOfLight/shape_representation_models.git
  abstract_other_model.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <shape_representation_models/abstract_other_model.h>

int ShapeRepresentationModels::AbstractOtherModel::getMethodType() const
{
  return ShapeRepresentationModels::AbstractModel::MethodType::OTHER;
}

