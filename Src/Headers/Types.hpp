#pragma once

#include <cstddef>
#include <vector>

class ExpressionType {
  public:
    enum class TypeClass {
      BASIC,
      ARRAY,
      STRUCT
    };

    static bool isEqual(const ExpressionType* one, const ExpressionType* other) {
        if (one->typeClass() != other->typeClass()) {
            return false;
        }

        switch (one->typeClass())
        {
            case TypeClass::BASIC:
                return BasicExprType::isEqual(one, other);
        
            case TypeClass::ARRAY:
                return ArrayVarType::isEqual(one, other);

            case TypeClass::STRUCT:
                return StructVarType::isEqual(one, other);
        }

        return false;
    }

    virtual ExpressionType* copy() const = 0;
    virtual size_t size() const = 0;
    virtual TypeClass typeClass() const = 0;

    virtual ~ExpressionType() = default;
};

class BasicExprType : public ExpressionType {
  public:
    static bool isEqual(const BasicExprType* one, const BasicExprType* other) {
        return one->basicType() == other->basicType();
    }

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

class FloatExprType : public BasicExprType {
  public:
    BasicType basicType() const override { return BasicType::FLOAT; }
    ExpressionType* copy() const override { return new FloatExprType(); }
};

class IntegerExprType : public BasicExprType {
  public:
    BasicType basicType() const override { return BasicType::INTEGER; }
    ExpressionType* copy() const override { return new IntegerExprType(); }
};

class StringExprType : public BasicExprType {
  public:
    BasicType basicType() const override { return BasicType::STRING; }
    size_t size() const override { return 8; } // sizeof(char*)
    ExpressionType* copy() const override { return new StringExprType(); }
};

class ArrayVarType : public ExpressionType {
  public:
    static bool isEqual(const ArrayVarType* one, const ArrayVarType* other) {
        return ExpressionType::isEqual(one, other) && 
               one->element_num_ == other->element_num_;
    }

  public:

    ArrayVarType(size_t element_num, ExpressionType* type) :
      element_num_(element_num),
      type_(type->copy()) { }

    size_t size() const override { return type_->size() * element_num_; }
    TypeClass typeClass() const override { return TypeClass::ARRAY; }

    ExpressionType* copy() const override { return new ArrayVarType(element_num_, type_); }

    ~ArrayVarType() {
        delete type_;
    }

  private:
    size_t element_num_;
    ExpressionType* type_;
};

class StructVarType : public ExpressionType {
  public:
    static bool isEqual(const StructVarType* one, const StructVarType* other) {
        if (one->fields_type_.size() != other->fields_type_.size()) {
            return false;
        }

        for (size_t ind = 0; ind < one->fields_type_.size(); ind++) {
            if (!ExpressionType::isEqual(one->fields_type_[ind],
                                         other->fields_type_[ind])) {
                return false;
            }
        }
        return true;
    }

  public:
    StructVarType(const std::vector<ExpressionType*>& fields_type)
    { 
        for (const ExpressionType* field_type : fields_type) {
            fields_type_.push_back(field_type->copy());
        }
    }

    size_t size() const override {
        size_t struct_size = 0;
        for (const ExpressionType* field_type : fields_type_) {
            struct_size += field_type->size();
        }
        return struct_size;
    }

    TypeClass typeClass() const override { return TypeClass::STRUCT; }

    ExpressionType* copy() const override { 
        return new StructVarType(fields_type_);
    }

    ~StructVarType() {
        for (ExpressionType* field_type : fields_type_) {
            delete field_type;
        }
    }

  private:
    std::vector<ExpressionType*> fields_type_;
};