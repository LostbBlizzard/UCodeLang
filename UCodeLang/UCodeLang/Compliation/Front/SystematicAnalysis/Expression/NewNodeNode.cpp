#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart
void SystematicAnalysis::OnNewNode(const NewExpresionNode* nod)
{
	if (_PassType == PassType::FixedTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = nod->_KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'new' keyword in safe mode.");
			return;
		}
	}

	TypeSymbol Type;
	Type_Convert(nod->_Type, Type);
	bool IsArray = nod->_Arrayexpression._Value.get();

	if (_PassType == PassType::FixedTypes)
	{
		if (IsArray)
		{
			TypeSymbol UintptrType(TypesEnum::uIntPtr);

			_LookingForTypes.push(UintptrType);
			OnExpressionTypeNode(nod->_Arrayexpression._Value.get(), GetValueMode::Read);

			if (!Type_CanBeImplicitConverted(_LastExpressionType, UintptrType))
			{
				auto  Token = _LastLookedAtToken;
				LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, UintptrType, true);
			}

			_LookingForTypes.pop();

		}

		auto Func = Type_GetFunc(Type, nod->_Parameters);
		_FuncToSyboID[Symbol_GetSymbolID(nod)] = Func;



		if (IsArray)
		{
			Type.SetAsAddressArray();
		}
		else
		{
			Type.SetAsAddress();
		}
		_LastExpressionType = Type;
	}

	if (_PassType == PassType::BuidCode)
	{


		auto Func = _FuncToSyboID.at(Symbol_GetSymbolID(nod));
		auto& ValuePars = nod->_Parameters;


		UAddress TypeSize;
		Type_GetSize(Type, TypeSize);


		if (IsArray)
		{

			TypeSymbol UintptrType = TypeSymbol();
			UAddress UintptrSize;
			{
				UintptrType.SetType(TypesEnum::uIntPtr);
				Type_GetSize(UintptrType, UintptrSize);
			}
			bool TypeHaveDestructor = Symbol_HasDestructor(Type);

			_LookingForTypes.push(UintptrType);


			OnExpressionTypeNode(nod->_Arrayexpression._Value.get(), GetValueMode::Read);

			auto Ex0 = _IR_LastExpressionField;
			IR_Build_ImplicitConversion(Ex0, _LastExpressionType, UintptrType);

			Ex0 = _IR_LastExpressionField;


			auto SizeV = IR_Load_UIntptr(TypeSize);//UintptrSize is for the array length for Drop 



			auto DataSize = IR_Build_Mult_uIntPtr(Ex0, SizeV);//uintptr 
			//malloc(sizeof(Type) * ArrayExpression); 


			if (TypeHaveDestructor)
			{
				IR_Build_Increment_uIntPtr(DataSize, UintptrSize);//Make room for Size on the Ptr
			}

			auto MallocPtr = _IR_LastExpressionField = _IR_LookingAtIRBlock->NewMallocCall(DataSize);

			if (TypeHaveDestructor)
			{
				_IR_LookingAtIRBlock->NewDereferenc_Store(MallocPtr, Ex0);//set Size.
				IR_Build_Increment_uIntPtr(MallocPtr, UintptrSize);//move pointer

			}

			//Call default on every

			//our index
			auto Indexir = IR_Load_UIntptr(0);

			size_t JumpLabel = _IR_LookingAtIRBlock->GetIndex();
			auto Cmpbool = _IR_LookingAtIRBlock->NewC_Equalto(Indexir, Ex0);


			auto JumpIns = _IR_LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

			auto OffsetIr = _IR_LookingAtIRBlock->New_Index_Vetor(MallocPtr, Indexir, SizeV);

			//loop on every
			bool IsPrimitiveType = Type_IsPrimitive(Type);

			if (!IsPrimitiveType)
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				_IR_LastExpressionField = OffsetIr;
			}

			IR_Build_FuncCall(Type, Func, ValuePars);

			if (IsPrimitiveType)
			{
				_IR_LookingAtIRBlock->NewDereferenc_Store(OffsetIr, _IR_LastExpressionField);
			}

			_IR_LookingAtIRBlock->New_Increment(Indexir);//index++



			_IR_LookingAtIRBlock->NewJump(JumpLabel);

			size_t ThisJumpLable = _IR_LookingAtIRBlock->GetIndex();
			_IR_LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);

			_IR_LastExpressionField = MallocPtr;

			_LookingForTypes.pop();



			_LastExpressionType = Type;
			_LastExpressionType._IsAddressArray = true;
		}
		else
		{
			auto SizeIR = IR_Load_UIntptr(TypeSize);
			auto MallocPtr = _IR_LookingAtIRBlock->NewMallocCall(SizeIR);
			_IR_LastExpressionField = MallocPtr;


			//Call ObjectNew
			if (Type_IsPrimitive(Type))
			{
				IR_Build_FuncCall(Type, Func, ValuePars);
				_IR_LookingAtIRBlock->NewDereferenc_Store(MallocPtr, _IR_LastExpressionField);
			}
			else
			{
				Func.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				IR_Build_FuncCall(Type, Func, ValuePars);
			}
			_IR_LastExpressionField = MallocPtr;

			_LastExpressionType = Type;
			_LastExpressionType._IsAddress = true;
		}

	}

}

UCodeLangFrontEnd