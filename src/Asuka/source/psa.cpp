//	Asuka - VirtualDub Build/Post-Mortem Utility
//	Copyright (C) 2005 Avery Lee
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "stdafx.h"
#include <vector>
#include <list>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <objbase.h>
#include <vd2/system/refcount.h>
#include <vd2/system/file.h>
#include <vd2/system/filesys.h>
#include <vd2/system/math.h>
#include <vd2/system/VDString.h>
#include <vd2/system/vdstl.h>

#pragma comment(lib, "d3dx9")

namespace {
	void DeleteShaderConstantTable(vdfastvector<uint32>& shader) {
		LPCVOID data;
		UINT size;
		if (D3D_OK == D3DXFindShaderComment((const DWORD *)shader.data(), MAKEFOURCC('C', 'T', 'A', 'B'), &data, &size)) {
			ptrdiff_t offset = (char *)data - (char *)shader.data();

			VDASSERT(!(offset & 3));
			VDASSERT(offset >= 8);

			// convert to dword offset
			offset >>= 2;
			size = (size + 3) >> 2;

			VDASSERT(offset + size <= shader.size());

			// erase comment token, fourcc, and comment data
			shader.erase(shader.begin() + (offset - 2), shader.begin() + offset + size);
		}
	}
}

void tool_psa(const vdfastvector<const char *>& args, const vdfastvector<const char *>& switches) {
	if (args.size() != 2) {
		puts("usage: asuka psa source.psa target.cpp");
		exit(5);
	}

	const char *filename = args[0];

	printf("Asuka: Assembling pixel shader (Direct3D): %s -> %s.\n", filename, args[1]);

	FILE *f = fopen(args[1], "w");
	if (!f) {
		printf("Couldn't open %s for write\n", args[1]);
		exit(10);
	}

	fprintf(f, "// Pixel shader data auto-generated by Asuka from %s. DO NOT EDIT!\n", VDFileSplitPath(filename));

	VDTextInputFile in(VDTextAToW(filename).c_str());

	vdfastvector<char> pstext;
	bool pscapture = false;
	bool hlslmode = false;
	VDStringA psname;
	int line = 0;

	for(;;) {
		const char *s = in.GetNextLine();
		++line;

		if (s && (*s == '\t' || *s == 0 || *s == ' ')) {
			while(*s == ' ' || *s == '\t')
				++s;

			if (pscapture) {
				pstext.insert(pstext.end(), s, s+strlen(s));
				pstext.push_back('\n');
			}
			continue;
		}

		if (s && !*s)
			continue;

		if (pscapture) {
			vdrefptr<ID3DXBuffer> pBuffer;
			vdrefptr<ID3DXBuffer> pErrors;
			HRESULT hr;
			
			if (hlslmode)
				hr = D3DXCompileShader(pstext.data(), pstext.size(), NULL, NULL, "main", "ps_2_0", 0, ~pBuffer, ~pErrors, NULL);
			else
				hr = D3DXAssembleShader(pstext.data(), pstext.size(), NULL, NULL, 0, ~pBuffer, ~pErrors);

			if (FAILED(hr)) {
				printf("Effect compilation failed for \"%s\"\n", filename);

				if (pErrors)
					puts((const char *)pErrors->GetBufferPointer());

				exit(10);
			}

			pErrors.clear();

			fprintf(f, "\n");

			const uint32 *src = (const uint32 *)pBuffer->GetBufferPointer();
			vdfastvector<uint32> shader(src, src + (pBuffer->GetBufferSize() >> 2));
			DeleteShaderConstantTable(shader);

			fprintf(f, "static const uint32 %s[]={\n", psname.c_str());

			if (hlslmode) {
				vdrefptr<ID3DXBuffer> disasm;
				hr = D3DXDisassembleShader((const DWORD *)shader.data(), FALSE, NULL, ~disasm);
				if (SUCCEEDED(hr)) {
					VDMemoryStream ms(disasm->GetBufferPointer(), disasm->GetBufferSize());
					VDTextStream ts(&ms);

					while(const char *s = ts.GetNextLine()) {
						fprintf(f, "\t// %s\n", s);
					}
				}
			}

			for(int i=0, count=shader.size(); i<count; i+=8) {
				fprintf(f, "\t");
				for(int j=i; j<i+8 && j<count; ++j) {
					fprintf(f, "0x%08x,", shader[j]);
				}
				fprintf(f, "\n");
			}
			fprintf(f, "};\n");
			pscapture = false;
		}

		if (!s)
			break;

		const char *t = s;
		if (isalpha((unsigned char)*t) || *t == '_') {
			++t;

			while(isalnum((unsigned char)*t) || *t == '_')
				++t;

			hlslmode = false;
			if (t[0] == ':' && t[1] == ':') {
				++t;
				hlslmode = true;
			}

			if (t[0] == ':' && t[1] == 0) {
				psname.assign(s, hlslmode ? t-1 : t);
				printf("\tExporting shader %s\n", psname.c_str());
				pscapture = true;
				pstext.clear();

				if (hlslmode) {
					VDStringA s;
					s.sprintf("#line %d \"%s\"\n", line + 1, filename);
					pstext.assign(s.begin(), s.end());
				}
				continue;
			}
		}

		printf("%s(%d,%d): parse error\n", filename, line, (t-s) + 1);
		exit(5);
	}

	printf("Asuka: Assembly was successful.\n");
}