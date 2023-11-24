#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnDropStatementNode(const DropStatementNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = node._KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant use 'drop' keyword in safe mode.");
		}
	}

	if (_PassType == PassType::BuidCode)
	{

		auto TypeToPush = TypeSymbol();
		TypeToPush.SetAsAddress();

		_LookingForTypes.push(TypeToPush);
	}

	OnExpressionTypeNode(node._expression._Value.get(), GetValueMode::Read);

	if (_PassType == PassType::BuidCode)
	{
		_LookingForTypes.pop();
	}

	auto Ex0 = _IR_LastExpressionField;
	auto Ex0Type = _LastExpressionType;
	if (_PassType == PassType::FixedTypes)
	{

		if (!(Ex0Type.IsAddress() || Ex0Type.IsAddressArray() || Ex0Type._IsDynamic))
		{
			auto Token = _LastLookedAtToken.value();
			LogError_ExpressionMustbeAnLocationValueError(Token, Ex0Type);
		}
	}
	if (_PassType == PassType::BuidCode)
	{

		bool TypeHaveDestructor = Symbol_HasDestructor(Ex0Type);
		if (Ex0Type.IsAddressArray())
		{
			Ex0Type._IsAddressArray = false;


			if (TypeHaveDestructor)
			{
				size_t UintptrSize;
				TypeSymbol UintptrType;
				{
					UintptrType = TypeSymbol();
					UintptrType.SetType(TypesEnum::uIntPtr);
					Type_GetSize(UintptrType, UintptrSize);
				}
				size_t TypeObjectSize;
				TypeSymbol TypeOfArr;
				{
					TypeOfArr = Ex0Type;
					TypeOfArr._IsAddressArray = false;
					Type_GetSize(TypeOfArr, TypeObjectSize);
				}

				auto StartArrPointer = _IR_LookingAtIRBlock->NewLoad(Ex0);
				IR_Build_Decrement_uIntPtr(Ex0, UintptrSize);//get the size the object
				auto ArrSize = _IR_LookingAtIRBlock->NewLoad_Dereferenc(Ex0, IR_ConvertToIRType(UintptrType));




				//Call default on every

				//our index
				auto Indexir = IR_Load_UIntptr(0);

				size_t JumpLabel = _IR_LookingAtIRBlock->GetIndex();
				auto Cmpbool = _IR_LookingAtIRBlock->NewC_Equalto(Indexir, ArrSize);


				auto JumpIns = _IR_LookingAtIRBlock->NewConditionalJump(Cmpbool, NullUInt64);

				auto OffsetIr = _IR_LookingAtIRBlock->New_Index_Vetor(StartArrPointer, Indexir, IR_Load_UIntptr(TypeObjectSize));

				//loop on every
				bool IsPrimitiveType = Type_IsPrimitive(TypeOfArr);


				{
					ObjectToDrop Data;
					Data._Object = OffsetIr;
					Data.Type = TypeOfArr;
					Data.DropType = ObjectToDropType::IRInstructionNoMod;
					IR_Build_DestructorCall(Data);//call on Object
				}


				_IR_LookingAtIRBlock->New_Increment(Indexir);//index++



				_IR_LookingAtIRBlock->NewJump(JumpLabel);

				size_t ThisJumpLable = _IR_LookingAtIRBlock->GetIndex();
				_IR_LookingAtIRBlock->UpdateConditionaJump(JumpIns, Cmpbool, ThisJumpLable);


				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}
			else
			{
				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}

			Ex0Type._IsAddressArray = true;
		}
		else
		{

			if (Ex0Type._IsDynamic)
			{

				auto ExPtr = Ex0Type.IsAddress() ? Ex0 : _IR_LookingAtIRBlock->NewLoadPtr(Ex0);

				auto IRStructType = IR_ConvertToIRType(Ex0Type);

				auto PtrMember = _IR_LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 0);
				auto VTableMember = _IR_LookingAtIRBlock->New_Member_Dereference(ExPtr, IRStructType, 1);

				auto Trait = Symbol_GetSymbol(Ex0Type).value();
				auto Traitinfo = Trait->Get_Info<TraitInfo>();

				size_t Index = 0;

				for (size_t i = 0; i < Traitinfo->_Funcs.size(); i++)
				{
					auto& Item = Traitinfo->_Funcs[i];
					if (Item.Syb->Get_Info<FuncInfo>()->_FuncType == FuncInfo::FuncType::Drop)
					{
						Index = i;
						break;
					}
				}
				auto DestructorFuncPtr = _IR_LookingAtIRBlock->New_Member_Dereference(VTableMember, IRType(_IR_Builder.ToID(Str_GetTraitVStructTableName(Trait->FullName))), Index);

				_IR_LookingAtIRBlock->NewPushParameter(PtrMember);
				_IR_LookingAtIRBlock->NewCallFuncPtr(DestructorFuncPtr);

				_IR_LookingAtIRBlock->NewFreeCall(PtrMember);
			}
			else
			{
				if (TypeHaveDestructor)
				{
					TypeSymbol tep = Ex0Type;
					if (tep.IsAddress())
					{
						tep._IsAddress = false;//DoDestructorCall will not do it if is IsPrimitive
					}

					ObjectToDrop Data;
					Data._Object = Ex0;
					Data.Type = tep;
					Data.DropType = ObjectToDropType::IRInstructionNoMod;
					IR_Build_DestructorCall(Data);//call on Object
				}

				_IR_LookingAtIRBlock->NewFreeCall(Ex0);
			}

		}
	}
}


UCodeLangFrontEnd

#endif