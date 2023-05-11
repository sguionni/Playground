#include "playground/oscillator.hpp"
#include <imgui/imgui-knobs.h>
#include <string>

namespace Playground
{
	void Oscillator::draw()
	{
		if ( ImGui::TreeNode( ( std::to_string( _id ) + " - " + getName() ).c_str() ) )
		{
			if ( ImGuiKnobs::KnobInt( "Shift", &_shift, -440, 440, 1, "%dhz", ImGuiKnobVariant_Tick ) )
			{
				refreshSample();
				// TODO: move cursor to avoid krrrrrrrr.
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob( "Detune", &_detune, -2.f, 2.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				refreshSample();
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob( "Amplitude", &_amplitude, 0.f, 1.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				refreshSample();
			}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob(
					 "Phase", &_phase, -std::numbers::pi, std::numbers::pi, 0.01f, "%.2f", ImGuiKnobVariant_Tick ) )
			{
				refreshSample();
			}
			ImGui::SameLine();
			ImGui::PlotLines( "", _sample, SAMPLE_SIZE, 0, "Wave", -1.0f, 1.0f, ImVec2( 400, 100 ) );
			ImGui::SameLine();
			ImGui::Checkbox( "Active", &_active );

			ImGui::TreePop();
		}
	}
} // namespace Playground
