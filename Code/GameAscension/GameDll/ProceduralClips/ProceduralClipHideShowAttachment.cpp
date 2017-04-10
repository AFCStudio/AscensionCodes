/* -------------------------------------------------------------
-	ProceduralClipHideShowAttachment.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class create a new procedural clip that
-	hide or show a attachment for previewed character
-	from mannequin editor at specified frame time.
-
-  -------------------------------------------------------------
-	Creation Date: 11.07.2014
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "StdAfx.h"

#include "ICryMannequin.h"

#include <Mannequin/Serialization.h>

struct SProceduralClipHideShowAttachmentParams : public IProceduralParams
{
	SProceduralClipHideShowAttachmentParams() : iHide(0)
	{
	}

	virtual void Serialize(Serialization::IArchive& ar)
	{
		ar(iHide, "Hide", "Hide attachment; 1 for hide, 0 for show");
		ar(Serialization::Decorators::AttachmentName<SProcDataCRC>(attachmentName), "AttachmentName", "Attachment name");
	}

	int				iHide;
	SProcDataCRC	attachmentName;
};

class SProceduralClipHideShowAttachment
	: public TProceduralClip < SProceduralClipHideShowAttachmentParams >
{
	IAttachment * attachment;

public:

	SProceduralClipHideShowAttachment() : attachment(NULL)
	{
	}

	~SProceduralClipHideShowAttachment()
	{
	}

	virtual void OnEnter(float blendTime, float duration, const SProceduralClipHideShowAttachmentParams& params)
	{
		attachment = m_charInstance->GetIAttachmentManager()->GetInterfaceByNameCRC(params.attachmentName.crc);

		if (attachment)
		{
			attachment->HideAttachment((uint32)params.iHide);
		}
	}
	virtual void OnExit(float blendTime)
	{
	}

	virtual void Update(float timePassed)
	{
	}
};

REGISTER_PROCEDURAL_CLIP(SProceduralClipHideShowAttachment, "HideShowAttachment");
