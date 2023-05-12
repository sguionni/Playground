#include "playground/filter.hpp"
#include <imgui/imgui-knobs.h>
#include <string>

namespace Playground
{
	void Filter::draw()
	{
		if ( ImGui::TreeNode( ( std::to_string( _id ) + " - " + getName() ).c_str() ) )
		{
			if ( ImGuiKnobs::KnobInt( "Cutoff", &_cutoff, 100, 2000, 1, "%dhz", ImGuiKnobVariant_Tick ) ) {}
			ImGui::SameLine();
			if ( ImGuiKnobs::Knob( "Q", &_resonance, 0.f, 10.f, 0.f, "%.2f", ImGuiKnobVariant_Tick ) ) {}
			ImGui::SameLine();
			ImGui::Checkbox( "Active", &_active );

			ImGui::TreePop();
		}
	}
} // namespace Playground
