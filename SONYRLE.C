static void unsonyRLE(uchar *src, uchar *dst)
{
	short someshort;
	long Lrunlen;
	int complete, partial;
	uchar *packdata, *packlen;
	ushort *buffer;
	
	static void *table[32] = {&&case2, &&case4, &&case6, &&case8, &&case10, &&case12,
							  &&case14, &&case16, &&case18, &&case20, &&case22, &&case24,
							  &&case26, &&case28, &&case30, &&case32, &&case34, &&case36,
							  &&case38, &&case40, &&case42, &&case44, &&case46, &&case48,
							  &&case50, &&case52, &&case54, &&case56, &&case58, &&case60,
							  &&case62, &&case64};
							  
	packdata = src;
	packlen = packdata + *(long*)packdata & 0x00FFFFFF;
	buffer = (ushort*)dst;
	
	for (packdata += 4; packdata < packlen; ) {
		someshort = *(((u_short *)packdata)++);
		
		if (someshort & 0x8000) {
			Lrunlen = someshort & 0x01FF;
			someshort &= 0x7E00;
			someshort = (someshort >> 1) | (someshort >> 9);
			complete = Lrunlen >> 6;
			partial = Lrunlen & 63;
			
			if (!partial) {
				partial = 64;
				complete--;
			}

			goto *table[(partial >> 1) - 1];
			
			case64:	*(((u_short *)buffer)++) = someshort;
			case62:	*(((u_short *)buffer)++) = someshort;
			case60:	*(((u_short *)buffer)++) = someshort;
			case58:	*(((u_short *)buffer)++) = someshort;
			case56:	*(((u_short *)buffer)++) = someshort;
			case54:	*(((u_short *)buffer)++) = someshort;
			case52:	*(((u_short *)buffer)++) = someshort;
			case50:	*(((u_short *)buffer)++) = someshort;
			case48:	*(((u_short *)buffer)++) = someshort;
			case46:	*(((u_short *)buffer)++) = someshort;
			case44:	*(((u_short *)buffer)++) = someshort;
			case42:	*(((u_short *)buffer)++) = someshort;
			case40:	*(((u_short *)buffer)++) = someshort;
			case38:	*(((u_short *)buffer)++) = someshort;
			case36:	*(((u_short *)buffer)++) = someshort;
			case34:	*(((u_short *)buffer)++) = someshort;
			case32:	*(((u_short *)buffer)++) = someshort;
			case30:	*(((u_short *)buffer)++) = someshort;
			case28:	*(((u_short *)buffer)++) = someshort;
			case26:	*(((u_short *)buffer)++) = someshort;
			case24:	*(((u_short *)buffer)++) = someshort;
			case22:	*(((u_short *)buffer)++) = someshort;
			case20:	*(((u_short *)buffer)++) = someshort;
			case18:	*(((u_short *)buffer)++) = someshort;
			case16:	*(((u_short *)buffer)++) = someshort;
			case14:	*(((u_short *)buffer)++) = someshort;
			case12:	*(((u_short *)buffer)++) = someshort;
			case10:	*(((u_short *)buffer)++) = someshort;
			case8:	*(((u_short *)buffer)++) = someshort;
			case6:	*(((u_short *)buffer)++) = someshort;
			case4:	*(((u_short *)buffer)++) = someshort;
			case2:	*(((u_short *)buffer)++) = someshort;
			
			if (complete--)
				goto case64;

		} 
		else *(((u_short *)buffer)++) = someshort;
	}
}  /* unsonyRLE */