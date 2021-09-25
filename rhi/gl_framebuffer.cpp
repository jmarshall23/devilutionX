// gl_framebuffer.cpp
//

#include "../source/diablo.h"
#include "image.h"
#include "gl_render.h"
#include "gl_framebuffer.h"
#include "glew/glew.h"

namespace devilution
{
	/*
	========================
	StormRenderTexture::StormRenderTexture
	========================
	*/
	StormRenderTexture::StormRenderTexture(StormImage* colorImage, StormImage* depthImage) {
		deviceHandle = -1;
		if (colorImage != nullptr)
		{
			AddRenderImage(colorImage);
		}
		this->depthImage = depthImage;
	}

	/*
	========================
	StormRenderTexture::~StormRenderTexture
	========================
	*/
	StormRenderTexture::~StormRenderTexture() {
		if (deviceHandle != -1)
		{
			glDeleteFramebuffers(1, &deviceHandle);
			deviceHandle = -1;
		}
	}
	/*
	================
	StormRenderTexture::AddRenderImage
	================
	*/
	void StormRenderTexture::AddRenderImage(StormImage* image) {
		if (deviceHandle != -1) {
			devilution::app_fatal("StormRenderTexture::AddRenderImage: Can't add render image after FBO has been created!");
		}

		colorImages.push_back(image);
	}

	/*
	================
	StormRenderTexture::InitRenderTexture
	================
	*/
	void StormRenderTexture::InitRenderTexture(void) {
		glGenFramebuffers(1, &deviceHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);

		bool isTexture3D = false;
		//if ((colorImages.Num() > 0 && colorImages[0]->GetOpts().textureType == TT_CUBIC) || ((depthImage != nullptr) && depthImage->GetOpts().textureType == TT_CUBIC))
		//{
		//	isTexture3D = true;
		//}

		if (!isTexture3D)
		{
			for (int i = 0; i < colorImages.size(); i++) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorImages[i]->GetDeviceHandle(0), 0);
			}

			if (depthImage != nullptr) {
				devilution::app_fatal("Depth texture not supportted yet");
			}

			if (colorImages.size() > 0)
			{
				GLenum DrawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
				if (colorImages.size() >= 5) {
					devilution::app_fatal("InitRenderTextures: Too many render targets!");
				}
				glDrawBuffers(colorImages.size(), &DrawBuffers[0]);
			}
		}
		else
		{
			devilution::app_fatal("3d textures not supported yet");
			//if (colorImages.Num() > 0)
			//{
			//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, colorImages[0]->texnum, 0);
			//}
			//
			//if (depthImage != nullptr) {
			//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthImage->texnum, 0);
			//}
		}


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			devilution::app_fatal("StormRenderTexture::InitRenderTexture: Failed to create rendertexture!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/*
	================
	StormRenderTexture::MakeCurrent
	================
	*/
	void StormRenderTexture::MakeCurrent(void) {
		glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);
	}

	/*
	================
	StormRenderTexture::BindNull
	================
	*/
	void StormRenderTexture::BindNull(void) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/*
	================
	StormRenderTexture::Resize
	================
	*/
	void StormRenderTexture::Resize(int width, int height) {
		//StormImage* target = nullptr;
		//
		//if (colorImages.Num() > 0) {
		//	target = colorImages[0];
		//}
		//else {
		//	target = depthImage;
		//}
		//
		//if (target->GetOpts().width == width && target->GetOpts().height == height) {
		//	return;
		//}
		//
		//for (int i = 0; i < colorImages.Num(); i++) {
		//	colorImages[i]->Resize(width, height);
		//}
		//
		//if (depthImage != nullptr) {
		//	depthImage->Resize(width, height);
		//}
		//
		//if (deviceHandle != -1)
		//{
		//	glDeleteFramebuffers(1, &deviceHandle);
		//	deviceHandle = -1;
		//}
		//
		//InitRenderTexture();
	}
}
