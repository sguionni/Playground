#include "playground/oscillator.hpp"
#include <imgui/imgui-knobs.h>
#include <string>

namespace Playground
{
	void Oscillator::_draw()
	{
		if ( _isOpen = ImGui::CollapsingHeader( getName().c_str() ), &_closable, ImGuiTreeNodeFlags_DefaultOpen )
		{
			if ( ImGuiKnobs::KnobInt( "Shift", &_shift, -440, 440, 1, "%dhz", ImGuiKnobVariant_Tick ) )
			{
				_refreshSample();
				// TODO: move cursor to avoid krrrrrrrr.
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob( "Detune", &_detune, -2.f, 2.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				_refreshSample();
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob( "Amplitude", &_amplitude, 0.f, 1.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				_refreshSample();
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob(
					 "Phase", &_phase, -std::numbers::pi, std::numbers::pi, 0.01f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				_refreshSample();
			}
			ImGui::SameLine();
			ImGui::PlotLines( "", _sample, SAMPLE_SIZE, 0, "", -1.0f, 1.0f, ImVec2( 400, 100 ) );
			ImGui::SameLine();
			ImGui::Checkbox( "Active", &_active );
			if ( ImGui::Button( "X" ) )
			{
				_delete();
			}
		}

		if ( _closable )
		{
			//_delete();
		}
	}
} // namespace Playground
