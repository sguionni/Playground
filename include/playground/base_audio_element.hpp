#ifndef __PLAYGROUND_BASE_AUDIO_ELEMENT__
#define __PLAYGROUND_BASE_AUDIO_ELEMENT__

#include <algorithm>
#include <concepts>
#include <functional>
#include <imgui/imgui.h>
#include <iostream>
#include <map>

namespace Playground
{
	constexpr size_t SAMPLE_RATE	  = 44100;
	constexpr size_t FRAME_PER_BUFFER = 256;

	class BaseAudioElement
	{
	  public:
		using CallbackDelete = std::function<void( const size_t )>;

		BaseAudioElement() { _id = BaseAudioElement::_counter++; }

		inline size_t id() const { return _id; }
		inline bool	  active() const { return _active; }

		inline virtual std::string getName() const = 0;
		inline virtual void		   reset() {}

		inline void setCallbackDelete( const CallbackDelete & p_callback ) { _callbackDelete = p_callback; }

		void draw()
		{
			ImGui::PushID( int( _id ) );
			_draw();
			ImGui::PopID();
		}

		template<typename K, class T>
		static inline void sharedPtrMapToWeakPtrMap( const std::map<K, std::shared_ptr<T>> & p_in,
													 std::map<K, std::weak_ptr<T>> &		 p_out )
		{
			std::transform( p_in.begin(),
							p_in.end(),
							std::inserter( p_out, p_out.end() ),
							[]( const std::pair<K, std::shared_ptr<T>> & p_ptr )
							{ return std::make_pair( p_ptr.first, std::weak_ptr<T>( p_ptr.second ) ); } );
		}

	  protected:
		static size_t _counter;

		size_t _id	   = 0;
		bool   _active = true;
		bool   _isOpen;
		bool   _closable = true;

		inline void _delete()
		{
			if ( _callbackDelete )
			{
				_callbackDelete( _id );
			}
		}

	  private:
		CallbackDelete _callbackDelete;

		virtual void _draw() = 0;
	};
} // namespace Playground

#endif
