/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_drawinglayer.hxx"

#include <drawinglayer/texture/texture.hxx>
#include <basegfx/numeric/ftools.hxx>
#include <basegfx/tools/gradienttools.hxx>
#include <basegfx/matrix/b2dhommatrixtools.hxx>

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvx::GeoTexSvx()
		{
		}

		GeoTexSvx::~GeoTexSvx()
		{
		}

		bool GeoTexSvx::operator==(const GeoTexSvx& /*rGeoTexSvx*/) const
		{
			// default implementation says yes (no data -> no difference)
			return true;
		}

		void GeoTexSvx::modifyBColor(const basegfx::B2DPoint& /*rUV*/, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
			// base implementation creates random color (for testing only, may also be pure virtual)
			rBColor.setRed((rand() & 0x7fff) / 32767.0);
			rBColor.setGreen((rand() & 0x7fff) / 32767.0);
			rBColor.setBlue((rand() & 0x7fff) / 32767.0);
		}

		void GeoTexSvx::modifyOpacity(const basegfx::B2DPoint& rUV, double& rfOpacity) const
		{
			// base implementation uses inverse of luminance of solved color (for testing only, may also be pure virtual)
			basegfx::BColor aBaseColor;
			modifyBColor(rUV, aBaseColor, rfOpacity);
			rfOpacity = 1.0 - aBaseColor.luminance();
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
    namespace texture
    {
		GeoTexSvxGradient::GeoTexSvxGradient(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 /* nSteps */, 
            double fBorder)
		:	GeoTexSvx(),
            maGradientInfo(),
            maTargetRange(rTargetRange),
			maStart(rStart),
			maEnd(rEnd),
			mfBorder(fBorder)
		{
		}

		GeoTexSvxGradient::~GeoTexSvxGradient()
		{
		}

		bool GeoTexSvxGradient::operator==(const GeoTexSvx& rGeoTexSvx) const
		{
			const GeoTexSvxGradient* pCompare = dynamic_cast< const GeoTexSvxGradient* >(&rGeoTexSvx);

            return (pCompare
				&& maGradientInfo == pCompare->maGradientInfo
				&& maTargetRange == pCompare->maTargetRange
				&& mfBorder == pCompare->mfBorder);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientLinear::GeoTexSvxGradientLinear(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fAngle)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createLinearODFGradientInfo(
                rTargetRange,
                nSteps,
                fBorder,
                fAngle);
		}

		GeoTexSvxGradientLinear::~GeoTexSvxGradientLinear()
		{
		}

        void GeoTexSvxGradientLinear::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
        {
            rOutmostColor = maStart;

            if(maGradientInfo.getSteps())
            {
                const double fStripeWidth(1.0 / maGradientInfo.getSteps());
                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    const double fPos(fStripeWidth * a);
                    // optimized below...
                    //
                    // basegfx::B2DHomMatrix aNew;
                    // aNew.scale(0.5, 0.5);
                    // aNew.translate(0.5, 0.5);
                    // aNew.scale(1.0, (1.0 - fPos));
                    // aNew.translate(0.0, fPos);
                    // aNew = maGradientInfo.getTextureTransform() * aNew;
                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * 
                        basegfx::tools::createScaleTranslateB2DHomMatrix(0.5, 0.5 * (1.0 - fPos), 0.5, 0.5 * (1.0 + fPos));
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maStart, maEnd, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
        }

		void GeoTexSvxGradientLinear::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getLinearGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientAxial::GeoTexSvxGradientAxial(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fAngle)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createAxialODFGradientInfo(
                rTargetRange,
                nSteps,
                fBorder,
                fAngle);
		}

		GeoTexSvxGradientAxial::~GeoTexSvxGradientAxial()
		{
		}

        void GeoTexSvxGradientAxial::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
        {
            rOutmostColor = maEnd;

            if(maGradientInfo.getSteps())
            {
                const double fStripeWidth(1.0 / maGradientInfo.getSteps());
                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    // const double fPos(fStripeWidth * a);
                    // optimized below...
                    //
                    // basegfx::B2DHomMatrix aNew;
                    // aNew.scale(0.50, (1.0 - fPos));
                    // aNew.translate(0.5, 0.0);
                    // aNew = maGradientInfo.getTextureTransform() * aNew;
                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * 
                        basegfx::tools::createScaleTranslateB2DHomMatrix(0.5, 1.0 - (fStripeWidth * a), 0.5, 0.0);
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maEnd, maStart, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
		}

		void GeoTexSvxGradientAxial::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getAxialGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientRadial::GeoTexSvxGradientRadial(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fOffsetX, 
            double fOffsetY)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createRadialODFGradientInfo(
                rTargetRange,
                basegfx::B2DVector(fOffsetX,fOffsetY),
                nSteps,
                fBorder);
		}

		GeoTexSvxGradientRadial::~GeoTexSvxGradientRadial()
		{
		}

		void GeoTexSvxGradientRadial::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
		{
            rOutmostColor = maStart;

            if(maGradientInfo.getSteps())
            {
                const double fStepSize(1.0 / maGradientInfo.getSteps());
                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    const double fSize(1.0 - (fStepSize * a));
                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * basegfx::tools::createScaleB2DHomMatrix(fSize, fSize);
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maStart, maEnd, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
        }

		void GeoTexSvxGradientRadial::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getRadialGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientElliptical::GeoTexSvxGradientElliptical(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fOffsetX, 
            double fOffsetY, 
            double fAngle)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createEllipticalODFGradientInfo(
                rTargetRange,
                basegfx::B2DVector(fOffsetX,fOffsetY),
                nSteps,
                fBorder,
                fAngle);
		}

		GeoTexSvxGradientElliptical::~GeoTexSvxGradientElliptical()
		{
		}

		void GeoTexSvxGradientElliptical::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
		{
            rOutmostColor = maStart;

            if(maGradientInfo.getSteps())
            {
                double fWidth(1.0);
                double fHeight(1.0);
                double fIncrementX(0.0);
                double fIncrementY(0.0);

                if(maGradientInfo.getAspectRatio() > 1.0)
                {
                    fIncrementY = fHeight / maGradientInfo.getSteps();
                    fIncrementX = fIncrementY / maGradientInfo.getAspectRatio();
                }
                else
                {
                    fIncrementX = fWidth / maGradientInfo.getSteps();
                    fIncrementY = fIncrementX * maGradientInfo.getAspectRatio();
                }

                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    // next step
                    fWidth -= fIncrementX;
                    fHeight -= fIncrementY;

                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * basegfx::tools::createScaleB2DHomMatrix(fWidth, fHeight);
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maStart, maEnd, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
		}

		void GeoTexSvxGradientElliptical::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getEllipticalGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientSquare::GeoTexSvxGradientSquare(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fOffsetX, 
            double fOffsetY, 
            double fAngle)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createSquareODFGradientInfo(
                rTargetRange,
                basegfx::B2DVector(fOffsetX,fOffsetY),
                nSteps,
                fBorder,
                fAngle);
		}

		GeoTexSvxGradientSquare::~GeoTexSvxGradientSquare()
		{
		}

		void GeoTexSvxGradientSquare::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
		{
            rOutmostColor = maStart;

            if(maGradientInfo.getSteps())
            {
                const double fStepSize(1.0 / maGradientInfo.getSteps());
                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    const double fSize(1.0 - (fStepSize * a));
                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * basegfx::tools::createScaleB2DHomMatrix(fSize, fSize);
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maStart, maEnd, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
		}

		void GeoTexSvxGradientSquare::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getSquareGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxGradientRect::GeoTexSvxGradientRect(
            const basegfx::B2DRange& rTargetRange, 
            const basegfx::BColor& rStart, 
            const basegfx::BColor& rEnd, 
            sal_uInt32 nSteps, 
            double fBorder, 
            double fOffsetX, 
            double fOffsetY, 
            double fAngle)
		:	GeoTexSvxGradient(rTargetRange, rStart, rEnd, nSteps, fBorder)
		{
            maGradientInfo = basegfx::tools::createRectangularODFGradientInfo(
                rTargetRange,
                basegfx::B2DVector(fOffsetX,fOffsetY),
                nSteps,
                fBorder,
                fAngle);
		}

		GeoTexSvxGradientRect::~GeoTexSvxGradientRect()
		{
		}

		void GeoTexSvxGradientRect::appendTransformationsAndColors(
            std::vector< B2DHomMatrixAndBColor >& rEntries, 
            basegfx::BColor& rOutmostColor)
		{
            rOutmostColor = maStart;

            if(maGradientInfo.getSteps())
            {
                double fWidth(1.0);
                double fHeight(1.0);
                double fIncrementX(0.0);
                double fIncrementY(0.0);

                if(maGradientInfo.getAspectRatio() > 1.0)
                {
                    fIncrementY = fHeight / maGradientInfo.getSteps();
                    fIncrementX = fIncrementY / maGradientInfo.getAspectRatio();
                }
                else
                {
                    fIncrementX = fWidth / maGradientInfo.getSteps();
                    fIncrementY = fIncrementX * maGradientInfo.getAspectRatio();
                }

                B2DHomMatrixAndBColor aB2DHomMatrixAndBColor;

                for(sal_uInt32 a(1); a < maGradientInfo.getSteps(); a++)
                {
                    // next step
                    fWidth -= fIncrementX;
                    fHeight -= fIncrementY;

                    aB2DHomMatrixAndBColor.maB2DHomMatrix = maGradientInfo.getTextureTransform() * basegfx::tools::createScaleB2DHomMatrix(fWidth, fHeight);
                    aB2DHomMatrixAndBColor.maBColor = interpolate(maStart, maEnd, double(a) / double(maGradientInfo.getSteps() - 1));
                    rEntries.push_back(aB2DHomMatrixAndBColor);
                }
            }
		}

		void GeoTexSvxGradientRect::modifyBColor(const basegfx::B2DPoint& rUV, basegfx::BColor& rBColor, double& /*rfOpacity*/) const
		{
            const double fScaler(basegfx::tools::getRectangularGradientAlpha(rUV, maGradientInfo));

            rBColor = basegfx::interpolate(maStart, maEnd, fScaler);
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxHatch::GeoTexSvxHatch(
            const basegfx::B2DRange& rTargetRange, 
            double fDistance, 
            double fAngle)
		:	mfDistance(0.1),
			mfAngle(fAngle),
			mnSteps(10L)
		{
			double fTargetSizeX(rTargetRange.getWidth());
			double fTargetSizeY(rTargetRange.getHeight());
			double fTargetOffsetX(rTargetRange.getMinX());
			double fTargetOffsetY(rTargetRange.getMinY());

            fAngle = -fAngle;

			// add object expansion
			if(0.0 != fAngle)
			{
				const double fAbsCos(fabs(cos(fAngle)));
				const double fAbsSin(fabs(sin(fAngle)));
				const double fNewX(fTargetSizeX * fAbsCos + fTargetSizeY * fAbsSin);
				const double fNewY(fTargetSizeY * fAbsCos + fTargetSizeX * fAbsSin);
				fTargetOffsetX -= (fNewX - fTargetSizeX) / 2.0;
				fTargetOffsetY -= (fNewY - fTargetSizeY) / 2.0;
				fTargetSizeX = fNewX;
				fTargetSizeY = fNewY;
			}

			// add object scale before rotate
			maTextureTransform.scale(fTargetSizeX, fTargetSizeY);

			// add texture rotate after scale to keep perpendicular angles
			if(0.0 != fAngle)
			{
				basegfx::B2DPoint aCenter(0.5, 0.5);
				aCenter *= maTextureTransform;

                maTextureTransform = basegfx::tools::createRotateAroundPoint(aCenter, fAngle)
                    * maTextureTransform;
			}

			// add object translate
			maTextureTransform.translate(fTargetOffsetX, fTargetOffsetY);

			// prepare height for texture
			const double fSteps((0.0 != fDistance) ? fTargetSizeY / fDistance : 10.0);
			mnSteps = basegfx::fround(fSteps + 0.5);
			mfDistance = 1.0 / fSteps;
		}

		GeoTexSvxHatch::~GeoTexSvxHatch()
		{
		}

		bool GeoTexSvxHatch::operator==(const GeoTexSvx& rGeoTexSvx) const
		{
			const GeoTexSvxHatch* pCompare = dynamic_cast< const GeoTexSvxHatch* >(&rGeoTexSvx);
			return (pCompare
				&& maTextureTransform == pCompare->maTextureTransform
				&& mfDistance == pCompare->mfDistance
				&& mfAngle == pCompare->mfAngle
				&& mnSteps == pCompare->mnSteps);
		}

		void GeoTexSvxHatch::appendTransformations(::std::vector< basegfx::B2DHomMatrix >& rMatrices)
		{
			for(sal_uInt32 a(1L); a < mnSteps; a++)
			{
				// create matrix
				const double fOffset(mfDistance * (double)a);
				basegfx::B2DHomMatrix aNew;
				aNew.set(1, 2, fOffset);
				rMatrices.push_back(maTextureTransform * aNew);
			}
		}

		double GeoTexSvxHatch::getDistanceToHatch(const basegfx::B2DPoint& rUV) const
		{
			const basegfx::B2DPoint aCoor(getBackTextureTransform() * rUV);
			return fmod(aCoor.getY(), mfDistance);
		}

        const basegfx::B2DHomMatrix& GeoTexSvxHatch::getBackTextureTransform() const
        {
            if(maBackTextureTransform.isIdentity())
            {
                const_cast< GeoTexSvxHatch* >(this)->maBackTextureTransform = maTextureTransform;
                const_cast< GeoTexSvxHatch* >(this)->maBackTextureTransform.invert();
            }

            return maBackTextureTransform;
        }
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace texture
	{
		GeoTexSvxTiled::GeoTexSvxTiled(
            const basegfx::B2DPoint& rTopLeft, 
            const basegfx::B2DVector& rSize)
		:	maTopLeft(rTopLeft),
			maSize(rSize)
		{
			if(basegfx::fTools::lessOrEqual(maSize.getX(), 0.0))
			{
				maSize.setX(1.0);
			}

			if(basegfx::fTools::lessOrEqual(maSize.getY(), 0.0))
			{
				maSize.setY(1.0);
			}
		}

		GeoTexSvxTiled::~GeoTexSvxTiled()
		{
		}

		bool GeoTexSvxTiled::operator==(const GeoTexSvx& rGeoTexSvx) const
		{
			const GeoTexSvxTiled* pCompare = dynamic_cast< const GeoTexSvxTiled* >(&rGeoTexSvx);
			return (pCompare
				&& maTopLeft == pCompare->maTopLeft
				&& maSize == pCompare->maSize);
		}

		void GeoTexSvxTiled::appendTransformations(::std::vector< basegfx::B2DHomMatrix >& rMatrices)
		{
			double fStartX(maTopLeft.getX());
			double fStartY(maTopLeft.getY());

			if(basegfx::fTools::more(fStartX, 0.0))
			{
				fStartX -= (floor(fStartX / maSize.getX()) + 1.0) * maSize.getX();
			}

			if(basegfx::fTools::less(fStartX + maSize.getX(), 0.0))
			{
				fStartX += floor(-fStartX / maSize.getX()) * maSize.getX();
			}

			if(basegfx::fTools::more(fStartY, 0.0))
			{
				fStartY -= (floor(fStartY / maSize.getY()) + 1.0) * maSize.getY();
			}

			if(basegfx::fTools::less(fStartY + maSize.getY(), 0.0))
			{
				fStartY += floor(-fStartY / maSize.getY()) * maSize.getY();
			}

			for(double fPosY(fStartY); basegfx::fTools::less(fPosY, 1.0); fPosY += maSize.getY())
			{
				for(double fPosX(fStartX); basegfx::fTools::less(fPosX, 1.0); fPosX += maSize.getX())
				{
					basegfx::B2DHomMatrix aNew;

					aNew.set(0, 0, maSize.getX());
					aNew.set(1, 1, maSize.getY());
					aNew.set(0, 2, fPosX);
					aNew.set(1, 2, fPosY);

					rMatrices.push_back(aNew);
				}
			}
		}
	} // end of namespace texture
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////
// eof
