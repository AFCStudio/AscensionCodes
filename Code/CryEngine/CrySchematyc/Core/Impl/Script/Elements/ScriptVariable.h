// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Schematyc/Script/Elements/IScriptVariable.h>
#include <Schematyc/SerializationUtils/MultiPassSerializer.h>

#include "Script/ScriptElementBase.h"
#include "Script/ScriptVariableData.h"

namespace Schematyc
{
// Forward declare classes.
class CAnyConstPtr;

class CScriptVariable : public CScriptElementBase<IScriptVariable>, public CMultiPassSerializer
{
public:

	CScriptVariable();
	CScriptVariable(const SGUID& guid, const char* szName, const SElementId& typeId, const SGUID& baseGUID);

	// IScriptElement
	virtual EScriptElementAccessor GetAccessor() const override;
	virtual void                   EnumerateDependencies(const ScriptDependencyEnumerator& enumerator, EScriptDependencyType type) const override;
	virtual void                   RemapDependencies(IGUIDRemapper& guidRemapper) override;
	virtual void                   ProcessEvent(const SScriptEvent& event) override;
	virtual void                   Serialize(Serialization::IArchive& archive) override;
	// ~IScriptElement

	// IScriptVariable
	virtual SElementId      GetTypeId() const override;
	virtual bool            IsArray() const override;
	virtual CAnyConstPtr    GetData() const override;
	virtual SGUID           GetBaseGUID() const override;
	virtual EOverridePolicy GetOverridePolicy() const override;
	// ~IScriptVariable

protected:

	// CMultiPassSerializer
	virtual void LoadDependencies(Serialization::IArchive& archive, const ISerializationContext& context) override;
	virtual void Load(Serialization::IArchive& archive, const ISerializationContext& context) override;
	virtual void Save(Serialization::IArchive& archive, const ISerializationContext& context) override;
	virtual void Edit(Serialization::IArchive& archive, const ISerializationContext& context) override;
	// ~CMultiPassSerializer

private:

	EScriptElementAccessor m_accessor = EScriptElementAccessor::Private;
	CScriptVariableData    m_data;
	SGUID                  m_baseGUID;
	EOverridePolicy        m_overridePolicy = EOverridePolicy::Default;
};
} // Schematyc
