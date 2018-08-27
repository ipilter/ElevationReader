#ifndef PPM_H_2D32DGH287D
#define PPM_H_2D32DGH287D

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <climits>

// image with 16 bit pixels
class Ppm
{
public:
  struct Pixel
  {
    typedef unsigned short ChannelType;

    ChannelType  mRed, mGreen, mBlue;

    Pixel( const ChannelType r = 0, const ChannelType g = 0, const ChannelType b = 0 )
      : mRed( r ), mGreen( g ), mBlue( b )
    { }

    static ChannelType maxColor()
    {
      return 0xffff; // maximum unsigned short value
    }

    Pixel& operator += (const Pixel& rhs)
    {
      mRed += rhs.mRed;
      mGreen += rhs.mGreen;
      mBlue += rhs.mBlue;
      return *this;
    }
    Pixel operator + (const Pixel& rhs)
    {
      return Pixel(mRed + rhs.mRed
                   , mGreen + rhs.mGreen
                   , mBlue + rhs.mBlue);
    }
    Pixel& operator -= (const Pixel& rhs)
    {
      mRed -= rhs.mRed;
      mGreen -= rhs.mGreen;
      mBlue -= rhs.mBlue;
      return *this;
    }
    Pixel operator - (const Pixel& rhs)
    {
      return Pixel(mRed - rhs.mRed
                   , mGreen - rhs.mGreen
                   , mBlue - rhs.mBlue);
    }
  };
  typedef std::vector<Pixel> ImageData;
  typedef ImageData::size_type SizeType;
public:
public:
  /*
    * Constructs a new image with given width, height and default pixel color
    */
  Ppm( const SizeType width, const SizeType height, const Pixel& color = Pixel() )
    : mWidth( width )
    , mHeight( height )
    , mImageData( mWidth * mHeight, color )
  { }
public:
  /*
    * Sets the pixel at the given coordinates
    */
  void setPixel( SizeType x, SizeType y, const Pixel& p )
  {
      mImageData[x + y * mWidth] = p;
  }
  /*
    * Returns the pixel at the given coordinates
    */
  Pixel getPixel( SizeType x, SizeType y) const
  {
      return mImageData[x + y * mWidth];
  }
public:
  /*
    * Returns the image width in pixels
    */
  SizeType width() const
  {
      return mWidth;
  }
  /*
    * Returns the image height in pixels
    */
  SizeType height() const
  {
      return mHeight;
  }
  /*
    * Returns the image size in bytes
    */
  SizeType size() const
  {
      return mImageData.size() * sizeof(Pixel);
  }
  /*
    * Returns the raw memory address of the first Pixel
    */
  Pixel* memory()
  {
    return &mImageData[0];
  }
public:
  /*
    * Writes the current image to the given file path
    */
  void write( const std::string& file ) const
  {
      std::ofstream stream( file.c_str(), std::ios::binary | std::ios::out );
      if( !stream.is_open() )
          throw std::runtime_error( std::string( "could not open ppm file: " ) + file );
      stream << "P6" << std::endl << mWidth << " " << mHeight << std::endl << (int)Pixel::maxColor() << std::endl;
      stream.write( reinterpret_cast<const char*>( &mImageData[0] ), mImageData.size() * sizeof( Pixel ) );
  }
private:
  const SizeType mWidth;
  const SizeType mHeight;
  ImageData mImageData;
};

#endif // PPM_H_2D32DGH287D