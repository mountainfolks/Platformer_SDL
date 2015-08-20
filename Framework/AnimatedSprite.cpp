// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
: Sprite()
, m_frameSpeed(0.0f)
, m_frameWidth(0)
, m_timeElapsed(0.0f)
, m_currentFrame(0)
, m_paused(false)
, m_loop(false)
, m_animating(false)
{

}

AnimatedSprite::~AnimatedSprite()
{

}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	m_frameWidth = 0;
	m_frameSpeed = 0;

	m_loop = false;
	m_paused = false;
	m_animating = true;

	Sprite::Initialise(texture);

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	m_frames.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	if (!m_paused){
		m_timeElapsed += deltaTime;
		if (m_timeElapsed > m_frameSpeed){
			m_timeElapsed = 0;
			m_currentFrame++;
			if (m_currentFrame >= m_frames.size()){
				m_currentFrame = 0;
			}
			if (!m_loop){
				m_paused = true;
			}
		}
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	backbuffer.DrawAnimatedSprite(m_frames[m_currentFrame], m_frameWidth, this->GetTexture());
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}