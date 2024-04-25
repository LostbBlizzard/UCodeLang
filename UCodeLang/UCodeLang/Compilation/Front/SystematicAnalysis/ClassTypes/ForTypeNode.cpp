#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"


UCodeLangFrontStart

void SystematicAnalysis::OnForTypeNode(const ForTypeNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;

	String ScopeName = ForTypeScope;
	if (IsgenericInstantiation)
	{
		ScopeName = _Generic_GenericSymbolStack.top()._IR_GenericFuncName;
	}
	
	_Table.AddScope(ScopeName);


	auto SymID = Symbol_GetSymbolID(node);

	_Table.RemoveScope();


	if (_PassType == PassType::GetTypes)
	{
		_Table.AddScope(ScopeName);

		auto& Sym = Symbol_AddSymbol(Isgeneric_t ? SymbolType::GenericForType : SymbolType::ForType, ForTypeScope, _Table._Scope.ThisScope, AccessModifierType::Default);
		_Table.AddSymbolID(Sym, SymID);
		Sym.NodePtr = &node;

		ForTypeInfo* info = new ForTypeInfo();
		Sym.Info.reset(info);

		info->Context = Save_SymbolContext();
		info->Context.value().Scope.ReMoveScope();
		Sym.PassState = _PassType;

		Generic_InitGenericalias(node._generic, IsgenericInstantiation, info->_GenericData);

		_Table.RemoveScope();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (node._generic._Values.size() == 0 || IsgenericInstantiation)
		{

			_Table.AddScope(ScopeName);
	
			size_t addedcount = node._typetoaddto._name._ScopedName.size();
			for (auto& Item : node._typetoaddto._name._ScopedName)
			{
				String itemstr;
				Token::PushString(itemstr,*Item._token);
				_Table.AddScope(itemstr);
			}
			
			UCodeLangDefer({ _Table.RemoveScope(); })

			UCodeLangDefer({
					for (size_t i = 0; i < addedcount; i++)
					{
						_Table.RemoveScope();
					}
			})
			{
				String c;
				node._typetoaddto._name.GetScopedName(ScopeName);
				ScopeName += c;
			}
			
			auto type = Type_ConvertAndValidateType(node._typetoaddto, NodeSyb_t::Any);

			bool hasattributes = type.IsAddress() || type.IsAddressArray() || type.IsDynamicTrait() || type.Isimmutable() || type.IsMovedType();
			if (hasattributes)
			{
				auto typenoattributes = type;
				Type_RemoveTypeattributes(typenoattributes);
			
				LogError(ErrorCodes::InValidType, "Cant add funcions to the type '" + ToString(type) + "' because of attributes. Did you mean '" +  ToString(typenoattributes) + "' ", NeverNullptr(node._typetoaddto._name._ScopedName.front()._token));
				return;
			}
			
			auto sym = Symbol_GetSymbol(SymID);
			if (sym->PassState != PassType::GetTypes)
			{
				return;
			}
			sym->PassState = PassType::FixedTypes;
			if (type.IsNull())
			{
				return;
			}
			sym->VarType = type;

			auto forinfo = sym->Get_Info<ForTypeInfo>();

			ClassStackInfo info;

			if (Type_IsPrimitive(type)) 
			{
				String primitiveforscope = ToString(type) + ForTypeScope;

				auto primitvesym = Symbol_GetSymbol(primitiveforscope,SymbolType::Type_alias);

				if (!primitvesym.has_value())
				{
					auto& sym = Symbol_AddSymbol(SymbolType::Type_alias, primitiveforscope, primitiveforscope, AccessModifierType::Public);
					_Table.AddSymbolID(sym, Symbol_GetSymbolID(&sym));
					sym.OutputIR = false;
					sym.VarType = type;

					primitvesym = &sym;
				}
				
				info.Syb = primitvesym.value().value();
			}
			else
			{
				info.Syb = Symbol_GetSymbol(type).value().value();
			}

			_ClassStack.push(info);

			_PassType = PassType::GetTypes;
			for (auto& Item : node._Nodes)
			{
				size_t index = _Table.Symbols.size();

				OnFuncNode(*Item);

				auto& newSym = _Table.Symbols[index];

				forinfo->Funcs.push_back(newSym.get());

			}

			if (!node._IsExport)
			{
				bool hasanyexport = false;
				for (auto& Item : forinfo->Funcs)
				{
					if (Item->Get_NodeInfo<FuncNode>()->_Signature._IsExport)
					{
						hasanyexport = true;
						break;
					}
				}

				if (hasanyexport)
				{
					LogError(ErrorCodes::InValidName, "cant export funcions because 'for type' is not exported"
						,NeverNullptr(node._typetoaddto._name._ScopedName.front()._token));
				}
			}

			_PassType = PassType::FixedTypes;
			for (auto& Item : node._Nodes)
			{
				OnFuncNode(*Item);
			}

			_ClassStack.pop();
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (!Isgeneric || IsgenericInstantiation)
		{

			auto basescope = _Table._Scope.ThisScope;
			_Table.AddScope(ScopeName);

			size_t addedcount = node._typetoaddto._name._ScopedName.size();
			for (auto& Item : node._typetoaddto._name._ScopedName)
			{
				String itemstr;
				Token::PushString(itemstr,*Item._token);
				_Table.AddScope(itemstr);
			}
			
			UCodeLangDefer({ _Table.RemoveScope(); })

			UCodeLangDefer({
					for (size_t i = 0; i < addedcount; i++)
					{
						_Table.RemoveScope();
					}
			})

			auto sym = Symbol_GetSymbol(SymID);
			sym->PassState = PassType::BuidCode;

			Vector<ClassMethod> methods;
			methods.reserve(node._Nodes.size());

			ClassStackInfo info;
			if (Type_IsPrimitive(sym->VarType)) 
			{
				String primitiveforscope = ToString(sym->VarType) + ForTypeScope;

				auto primitvesym = Symbol_GetSymbol(primitiveforscope,SymbolType::Type_alias);
				
				info.Syb = primitvesym.value().value();
			}
			else
			{
				info.Syb = Symbol_GetSymbol(sym->VarType).value().value();
			}



			_ClassStack.push(info);
			for (auto& Item : node._Nodes)
			{
				OnFuncNode(*Item);
				
				auto t = Item->_Signature._Name.token;
				
				auto FullName = _Table._Scope.ThisScope;
					switch (t->Type)
					{
					case TokenType::Name:
					{
						ScopeHelper::GetApendedString(FullName, t->Value._String);
					}
					break;
					case TokenType::plus:
					{
						ScopeHelper::GetApendedString(FullName, Overload_Plus_Func);
					}
					break;
					case TokenType::CompoundAdd:
					{
						ScopeHelper::GetApendedString(FullName, Overload_Compoundplus_Func);
					}
					break;
					default:
						UCodeLangUnreachable();
						break;
					}

				Class_Data* Ptr = Assembly_GetAssemblyClass(RemoveSymboolFuncOverloadMangling(FullName));

				auto last = std::move(Ptr->Methods.back());
				Ptr->Methods.pop_back();
				methods.push_back(std::move(last));
			}
			_ClassStack.pop();


			auto& OutInfo = _Lib.Get_Assembly().AddForType(ScopeName, ScopeName);
			OutInfo._TargetType = Assembly_ConvertToType(sym->VarType);
			OutInfo._AddedMethods = std::move(methods);
			OutInfo.IsExported = node._IsExport;
			
			OutInfo._Scope = basescope;
		}	
	}
}


UCodeLangFrontEnd
#endif
