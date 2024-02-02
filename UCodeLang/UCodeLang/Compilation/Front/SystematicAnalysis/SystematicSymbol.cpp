#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void  SystematicAnalysis::Symbol_Update_ClassSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ClassInfo* Vp = Sym->Get_Info<ClassInfo>();
		auto& classNode = *Sym->Get_NodeInfo<ClassNode>();
		if (!Symbol_IsDependencies(Vp))
		{
			auto Old = SaveAndMove_SymbolContext();
			Set_SymbolContext(Vp->Context.value());

			OnClassNode(classNode);//update class fields


			Set_SymbolContext(std::move(Old));
		}
		else
		{
			if (_ClassDependencies.size() > 1) {
				LogError_TypeDependencyCycle(NeverNullptr(classNode._className.token), Vp);
			}
		}

		/*
		ClassInfo* info = Sym->Get_Info<ClassInfo>();
		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnClassNode(*Sym->Get_NodeInfo<ClassNode>());

		Set_SymbolContext(std::move(OldContext));
		*/
	}
}
void  SystematicAnalysis::Symbol_Update_FuncSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		FuncInfo* info = Sym->Get_Info<FuncInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnFuncNode(*Sym->Get_NodeInfo<FuncNode>());

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_EnumSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		EnumInfo* info = Sym->Get_Info<EnumInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnEnum(*Sym->Get_NodeInfo<EnumNode>());

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_TraitSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TraitInfo* info = Sym->Get_Info<TraitInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnTrait(*Sym->Get_NodeInfo<TraitNode>());

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_TagSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		TagInfo* info = Sym->Get_Info<TagInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnTag(*Sym->Get_NodeInfo<TagTypeNode>());

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_AliasSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		if (Sym->Type == SymbolType::Generic_Alias)
		{
			Generic_AliasInfo* info = Sym->Get_Info<Generic_AliasInfo>();

			auto OldContext = SaveAndMove_SymbolContext();
			Set_SymbolContext(info->Context.value());

			OnAliasNode(*Sym->Get_NodeInfo<AliasNode>());

			Set_SymbolContext(std::move(OldContext));
		}
		else
		{
			AliasInfo* info = Sym->Get_Info<AliasInfo>();

			auto OldContext = SaveAndMove_SymbolContext();
			Set_SymbolContext(info->Context.value());

			OnAliasNode(*Sym->Get_NodeInfo<AliasNode>());

			Set_SymbolContext(std::move(OldContext));
		}
	}
}
void SystematicAnalysis::Symbol_Update_EvalSym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ConstantExpressionInfo* info = Sym->Get_Info<ConstantExpressionInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnDeclareVariablenode(*Sym->Get_NodeInfo<DeclareVariableNode>(), DeclareStaticVariableNode_t::Eval);

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_ThreadAndStatic_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		UCodeLangAssert(Sym->Type == SymbolType::ThreadVarable || Sym->Type == SymbolType::StaticVarable);
		DeclareVariableInfo* info = Sym->Get_Info<DeclareVariableInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnDeclareVariablenode(*Sym->Get_NodeInfo<DeclareVariableNode>(),
			Sym->Type == SymbolType::ThreadVarable ? DeclareStaticVariableNode_t::Thread : DeclareStaticVariableNode_t::Static);

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_ForType_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	if (Sym->PassState == PassType::GetTypes)
	{
		ForTypeInfo* info = Sym->Get_Info<ForTypeInfo>();

		auto OldContext = SaveAndMove_SymbolContext();
		Set_SymbolContext(info->Context.value());

		OnForTypeNode(*Sym->Get_NodeInfo<ForTypeNode>());

		Set_SymbolContext(std::move(OldContext));
	}
}
void SystematicAnalysis::Symbol_Update_Sym_ToFixedTypes(NeverNullPtr<Symbol> Sym)
{
	switch (Sym->Type)
	{
	case SymbolType::Generic_class:
	case SymbolType::Type_class:
		Symbol_Update_ClassSym_ToFixedTypes(Sym);
		break;



	case SymbolType::Hard_Type_alias:
	case SymbolType::Type_alias:
	case SymbolType::Func_ptr:
	case SymbolType::Hard_Func_ptr:
	case SymbolType::Generic_Alias:
		Symbol_Update_AliasSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Enum:
	case SymbolType::Enum:
		Symbol_Update_EnumSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Tag_class:
	case SymbolType::Generic_Tag:
		Symbol_Update_TagSym_ToFixedTypes(Sym);
		break;

	case SymbolType::ConstantExpression:
		Symbol_Update_EvalSym_ToFixedTypes(Sym);
		break;

	case SymbolType::Generic_Trait:
	case SymbolType::Trait_class:
		Symbol_Update_TraitSym_ToFixedTypes(Sym);
		break;

	case SymbolType::GenericFunc:
	case SymbolType::Func:
		Symbol_Update_FuncSym_ToFixedTypes(Sym);
		break;

	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		Symbol_Update_ThreadAndStatic_ToFixedTypes(Sym);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

Vector<Symbol*>& SystematicAnalysis::GetSymbolsWithName(const String_view& Name)
{
	if (_PassType != PassType::GetTypes) {
		for (size_t i = 0; i < NodeCompileTimeIfs.size(); i++)
		{
			auto& Item = NodeCompileTimeIfs[i];

			bool mayhavesymbol = false;
			for (auto& Str : Item.PossibleSymbolNames)
			{
				if (StringHelper::Contains(Str, Name))
				{
					mayhavesymbol = true;
					break;
				}
			}

			if (mayhavesymbol)
			{
				auto oldcontext = SaveAndMove_SymbolContext();
				Set_SymbolContext(Item.node.Context);

				OnCompileTimeIfNode(*Item.node.node, false);

				Set_SymbolContext(std::move(oldcontext));

				i--;//Item Was Removed from list

			}
		}
	}

	return  _Table.GetSymbolsWithName(Name);
}
const Vector<const Symbol*>& SystematicAnalysis::GetSymbolsWithName(const String_view& Name) const
{
	return  _Table.GetSymbolsWithName(Name);
}
void SystematicAnalysis::Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const NeverNullPtr<Token> Value)
{
	Symbol_RedefinitionCheck(Syb->FullName, Syb->Type, Value);
}
void SystematicAnalysis::Symbol_RedefinitionCheck(const String_view FullName, SymbolType Type, const NeverNullPtr<Token> Value)
{
	auto other = Symbol_GetSymbol(FullName, Type);
	if (other)
	{
		if (other.value()->FullName == FullName) {
			LogError_SymbolRedefinition(Value, other.value());
		}
	}
}
void SystematicAnalysis::Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const FuncInfo* Fvalue, const NeverNullPtr<Token> Value)
{
	auto other = Symbol_GetSymbol(Syb->FullName, Syb->Type).value();
	if (other != Syb)
	{
		bool Good = false;

		if (other->Type == SymbolType::Func)
		{
			FuncInfo* SybValue = other->Get_Info<FuncInfo>();


			/*if (!AreTheSameWithOutimmutable(Fvalue->Ret, SybValue->Ret))
			{
				goto GoodJump;
			}
			*/

			if (SybValue->Pars.size() == Fvalue->Pars.size())
			{

				for (size_t i = 0; i < Fvalue->Pars.size(); i++)
				{
					auto& V1 = Fvalue->Pars[i];
					auto& V2 = SybValue->Pars[i];

					if (!Type_AreTheSame(V1.Type, V2.Type) || V1.IsOutPar != V2.IsOutPar)
					{

						return;
					}
				}
				goto GoodJump;
			}
			else
			{
				return;
			}
		}

		if (other->Type == SymbolType::GenericFunc)
		{
			Good = true;
		}
	GoodJump:
		if (!Good)
		{
			LogError_SymbolRedefinition(Value, Syb);
		}
	}
}
bool SystematicAnalysis::Symbol_IsVarableType(SymbolType type) const
{
	switch (type)
	{
	case SymbolType::ParameterVarable:
	case SymbolType::StackVarable:
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
	case SymbolType::Unmaped_Varable:
		return true;
	default:
		return false;
		break;
	}
}

UCodeLangFrontEnd

#endif