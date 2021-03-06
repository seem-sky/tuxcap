{
	if (!LockSurface())
		return;

	int aFPSrcY = (int)(theSrcRect.mY * 0x10000);

	int anAddX = (int)((theSrcRect.mWidth * 0x10000) / theDestRect.mWidth);
	int anAddY = (int)((theSrcRect.mHeight * 0x10000) / theDestRect.mHeight);

	if (theColor == Color::White)
	{
		if (mSurface->format->BitsPerPixel == 16)
		{
                //FIXME replace with Uint32 and Uint16
			ushort* aDestPixelsRow = ((ushort*) mSurface->pixels) + (theDestRect.mY * mSurface->pitch/2) + theDestRect.mX;

			uint32_t aRMask = mSurface->format->Rmask;
			uint32_t aGMask = mSurface->format->Gmask;
			uint32_t aBMask = mSurface->format->Bmask;

			//uint32_t aRRoundAdd = aRMask >> 1;
			//uint32_t aGRoundAdd = aGMask >> 1;
			//uint32_t aBRoundAdd = aBMask >> 1;

			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = (int)(theSrcRect.mX * 0x10000);

				ushort* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->GetWidth() * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);

					uint32_t src = READ_COLOR(aSrcPixels);
					uint32_t dest = *aDestPixels;

					int a = src >> 24;

					if (a != 0)
					{
						int oma = 256 - a;

						*(aDestPixels _PLUSPLUS) =
							src +
							(((((dest & aRMask) * oma)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mSurface->pitch/2;
				aFPSrcY += anAddY;
			}
		}
		else if (mSurface->format->BitsPerPixel == 32)
		{
			uint32_t* aDestPixelsRow = ((uint32_t*) mSurface->pixels) + (theDestRect.mY * mSurface->pitch/4) + theDestRect.mX;

			uint32_t aRMask = mSurface->format->Rmask;
			uint32_t aGMask = mSurface->format->Gmask;
			uint32_t aBMask = mSurface->format->Bmask;

			//uint32_t aRRoundAdd = aRMask >> 1;
			//uint32_t aGRoundAdd = aGMask >> 1;
			//uint32_t aBRoundAdd = aBMask >> 1;

			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = (int)(theSrcRect.mX * 0x10000);

				uint32_t* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->GetWidth() * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);

					uint32_t src = READ_COLOR(aSrcPixels);
					uint32_t dest = *aDestPixels;

					int a = src >> 24;

					if (a != 0)
					{
						int oma = 256 - a;

						*(aDestPixels _PLUSPLUS) =
							src +
							(((((dest & aRMask) * oma)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mSurface->pitch/4;
				aFPSrcY += anAddY;
			}
		}
	}
	else
	{
		int ca = theColor.mAlpha;
		int cr = (theColor.mRed * ca) / 255;
		int cg = (theColor.mGreen * ca) / 255;
		int cb = (theColor.mBlue * ca) / 255;

		if (mSurface->format->BitsPerPixel == 16)
		{
			ushort* aDestPixelsRow = ((ushort*) mSurface->pixels) + (theDestRect.mY * mSurface->pitch/2) + theDestRect.mX;


			uint32_t aRMask = mSurface->format->Rmask;
			uint32_t aGMask = mSurface->format->Gmask;
			uint32_t aBMask = mSurface->format->Bmask;

			//uint32_t aRRoundAdd = aRMask >> 1;
			//uint32_t aGRoundAdd = aGMask >> 1;
			//uint32_t aBRoundAdd = aBMask >> 1;

			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = (int)(theSrcRect.mX * 0x10000);

				ushort* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->GetWidth() * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);

					uint32_t src = READ_COLOR(aSrcPixels);
					uint32_t dest = *aDestPixels;

					int a = ((src >> 24) * ca) / 255;

					if (a != 0)
					{
						int oma = 256 - a;

						*(aDestPixels _PLUSPLUS) =
							(((((dest & aRMask) * oma) + ((src & aRMask) * cr)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma) + ((src & aGMask) * cg)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma) + ((src & aBMask) * cb)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mSurface->pitch/2;
				aFPSrcY += anAddY;
			}
		}
		else if (mSurface->format->BitsPerPixel == 32)
		{
			uint32_t* aDestPixelsRow = ((uint32_t*) mSurface->pixels) + (theDestRect.mY * mSurface->pitch/4) + theDestRect.mX;


			uint32_t aRMask = mSurface->format->Rmask;
			uint32_t aGMask = mSurface->format->Gmask;
			uint32_t aBMask = mSurface->format->Bmask;

			//uint32_t aRRoundAdd = aRMask >> 1;
			//uint32_t aGRoundAdd = aGMask >> 1;
			//uint32_t aBRoundAdd = aBMask >> 1;

			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = (int)(theSrcRect.mX * 0x10000);

				uint32_t* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->GetWidth() * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);

					uint32_t src = READ_COLOR(aSrcPixels);
					uint32_t dest = *aDestPixels;

					int a = ((src >> 24) * ca) / 255;
					if (a != 0)
					{
						int oma = 256 - a;

						*(aDestPixels _PLUSPLUS) =
							(((((dest & aRMask) * oma) + ((src & aRMask) * cr)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma) + ((src & aGMask) * cg)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma) + ((src & aBMask) * cb)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mSurface->pitch/4;
				aFPSrcY += anAddY;
			}
		}
	}

	UnlockSurface();
}

