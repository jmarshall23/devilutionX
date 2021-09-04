/**
 * @file textdat.cpp
 *
 * Implementation of all dialog texts.
 */
#include "textdat.h"
#include "utils/language.h"

#include "datatable.h"

namespace devilution {

	/** Contains the data related to each speech ID. */
	TextDataStruct *Texts;

	void InitTextDataStruct(void)
    {
	    Texts = new TextDataStruct[textTable->NumRows()];

		for (int i = 0; i < textTable->NumRows(); i++) {
		    Texts[i].txtstr = textTable->GetValue("text", i);
		    Texts[i].scrlltxt = textTable->GetInt("scrlltxt", i);
		    Texts[i].sfxnr = SfxIdFromString(textTable->GetValue("sfxnr", i));
		}
	}
} // namespace devilution
