#pragma once

#include <cstddef>
#include <vector>

class VarType {
  public:

    enum class TypeClass {
      BASIC,
      ARRAY,
      STRUCT
    };

    virtual size_t size() const = 0;
    virtual TypeClass typeClass() const = 0;
};

class BasicVarType : public VarType {
  public:
    enum class BasicType {
      INTEGER,
      FLOAT,
      STRING
    };

    size_t size() const override { return 4; }
    TypeClass typeClass() const override { return TypeClass::BASIC; }

    virtual BasicType basicType() const = 0;
};

class FloatType : public BasicVarType {
  public:
    size_t size() const override { return 4; }
    BasicType basicType() const override { return BasicType::FLOAT; }
};

class IntegerType : public BasicVarType {
  public:
    BasicType basicType() const override { return BasicType::INTEGER; }
};

class StringType : public BasicVarType {
  public:
    BasicType basicType() const override { return BasicType::STRING; }
    size_t size() const override { return 8; } // sizeof(char*)
};

class ArrayVarType : public VarType {
  public:
    ArrayVarType(size_t element_num, VarType* type) :
      element_num_(element_num),
      type_(type) { }

    size_t size() const override { return type_->size() * element_num_; }
    TypeClass typeClass() const override { return TypeClass::ARRAY; }

  private:
    size_t element_num_;
    VarType* type_;
};

class StructVarType : public VarType {
  public:
    StructVarType(std::vector<VarType*> fields_type) : 
      fields_type_(fields_type) { }

    size_t size() const override {
      size_t struct_size = 0;
      for (const VarType* field_type : fields_type_) {
        struct_size += field_type->size();
      }
      return struct_size;
    }

    TypeClass typeClass() const override { return TypeClass::STRUCT; }

  private:
    std::vector<VarType*> fields_type_;
};