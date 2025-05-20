#include "Types.hpp"

bool ExpressionType::isEqual(const ExpressionType* one, const ExpressionType* other)
{
    if (one->typeClass() != other->typeClass()) {
        return false;
    }

    switch (one->typeClass())
    {
        case TypeClass::BASIC:
            return BasicExprType::isEqual(static_cast<const BasicExprType*>(one),
                                          static_cast<const BasicExprType*>(other));
    
        case TypeClass::ARRAY:
            return ArrayVarType::isEqual(static_cast<const ArrayVarType*>(one),
                                         static_cast<const ArrayVarType*>(other));

        case TypeClass::STRUCT:
            return StructVarType::isEqual(static_cast<const StructVarType*>(one),
                                          static_cast<const StructVarType*>(other));
    }

    return false;
}