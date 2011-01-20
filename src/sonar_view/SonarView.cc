/* 
 * File:   SonarViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#include <stdexcept>
#include "SonarView.h"
#include "SonarViewGL.h"
#include <QtCore/QtPlugin>
#include "SonarViewGL.h"

SonarView::SonarView(QWidget *parent,bool use_openGL):
ImageView(parent,false)
{
	setOpenGL(use_openGL);
}


SonarView::~SonarView()
{
}

void SonarView::setOpenGL(bool flag)
{
  if(flag)
  {
    //prevent activating if it is already activated
    if(image_view_gl)
      return;

    image_view_gl = new SonarViewGL(this);
    if (!image_view_gl)
      return;
    image_view_gl->resize(width(),height());
    //image_view_gl->setAspectRatio(aspect_ratio);
    image_view_gl->show();
    SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
    window->reset(0.024); //TODO hardcoded value
  }
  else
  {
    delete image_view_gl;
    image_view_gl = NULL;
  }
   
}

void SonarView::setSonarScan2(base::samples::SonarScan *scan){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	window->setData(scan->scanData,scan->angle/2.0*M_PI*6399.0);
}

void SonarView::setSonarScan(const char *data_, int size, double angle, bool fromBearing){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	double bearing =angle;
	if(!fromBearing)
		bearing = angle/2.0*M_PI*6399.0;
	
	std::vector<uint8_t> data;
	for(int i=0;i<size;i++){
		data.push_back(data_[i]);
	}
	window->setData(data,bearing);
}

void SonarView::keyPressEvent ( QKeyEvent * event ){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	window->keyPressEvent(event);	
}