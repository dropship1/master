#include "input/commandhandler.hpp"

using namespace bright::input;


CommandHandler::CommandHandler(): controlState_(){
}


void CommandHandler::handle_command(std::shared_ptr<bright::input::CommandEvent> pCommandEvent){

  std::istringstream in( pCommandEvent->command_string() );
  std::string controlName;
  std::string commandName;
  std::string controlType;
  std::string data;
  in >> controlName;
  in >> commandName;
  in >> controlType;
  in >> data;
  CommandMessage cmdMessage(controlName, commandName, controlType, data);

  if( cmdMessage.command_name().compare("STATE") == 0 ){
  
    if( cmdMessage.control_type().compare("STATE_ON")  == 0 ){
      if( cmdMessage.control_name().compare("MOVE_FORWARD") == 0 ){
        controlState_.moveForward_ = true;
      }
      else if( cmdMessage.control_name().compare("MOVE_BACK") == 0 ){
        controlState_.moveBackward_ = true;
      }
      else if( cmdMessage.control_name().compare("MOVE_LEFT") == 0 ){
        controlState_.moveLeft_ = true;
      }
      else if( cmdMessage.control_name().compare("MOVE_RIGHT") == 0 ){
        controlState_.moveRight_ = true;
      }
    }
    else if( cmdMessage.control_type().compare("STATE_OFF") == 0 ){
      if( cmdMessage.control_name().compare("MOVE_FORWARD") == 0 ){
        controlState_.moveForward_ = false; 
      }
      else if( cmdMessage.control_name().compare("MOVE_BACK") == 0 ){
        controlState_.moveBackward_ = false;
      }
      else if( cmdMessage.control_name().compare("MOVE_LEFT") == 0 ){
        controlState_.moveLeft_ = false;
      }
      else if( cmdMessage.control_name().compare("MOVE_RIGHT") == 0 ){
        controlState_.moveRight_ = false;
      }
    }
  
  }
  else if( cmdMessage.command_name().compare("AXIS") == 0 ){
    std::string::size_type sz;
    int amount = std::stoi (cmdMessage.control_type(),&sz);

    if( amount > 0 ){
      if( cmdMessage.control_name().compare("CAMERA_X_AXIS") == 0 ){
         controlState_.rotations_.push_back("r");
      }
      else if( cmdMessage.control_name().compare("CAMERA_Y_AXIS") == 0 ){
         controlState_.rotations_.push_back("d");
      }
    }
    else if(  amount < 0  ){
      if( cmdMessage.control_name().compare("CAMERA_X_AXIS") == 0 ){
         controlState_.rotations_.push_back("l");
      }
      else if( cmdMessage.control_name().compare("CAMERA_Y_AXIS") == 0 ){
         controlState_.rotations_.push_back("u");
      }
    }

  }


}


bool CommandHandler::more_commands(){
  return !commands_.empty();
}


CommandMessage CommandHandler::get_next_command(){
  if ( !commands_.empty() ){
    return commands_[0];
  }
  return CommandMessage("EMTPY", "EMPTY", "EMPTY", "EMTPY");
}


void CommandHandler::remove_one_command(){
  if ( !commands_.empty() ){
    commands_.erase( commands_.begin() );
  }
}


void CommandHandler::update(){

  if( controlState_.moveForward_ ){
    commands_.push_back( CommandMessage("MOVE_FORWARD", "STATE", "STATE_ON", "") );
  }
  if( controlState_.moveBackward_ ){
    commands_.push_back( CommandMessage("MOVE_BACK", "STATE", "STATE_ON", "") );
  }
  if( controlState_.moveLeft_ ){
    commands_.push_back( CommandMessage("MOVE_LEFT", "STATE", "STATE_ON", "") );
  }
  if( controlState_.moveRight_ ){
    commands_.push_back( CommandMessage("MOVE_RIGHT", "STATE", "STATE_ON", "") );
  }

  if( !controlState_.rotations_.empty() ){

    auto handle_rotation = [&] (std::string& rotation) { 
      if( rotation.compare("l") == 0 ){
        commands_.push_back( CommandMessage("CAMERA_X_AXIS", "AXIS", "-1", "") );
      }
      else if( rotation.compare("r") == 0 ){
        commands_.push_back( CommandMessage("CAMERA_X_AXIS", "AXIS", "1", "") );
      }
      else if( rotation.compare("u") == 0 ){
        commands_.push_back( CommandMessage("CAMERA_Y_AXIS", "AXIS", "-1", "") );
      }
      else if( rotation.compare("d") == 0 ){
        commands_.push_back( CommandMessage("CAMERA_Y_AXIS", "AXIS", "1", "") );
      }
    };
    std::for_each(controlState_.rotations_.begin(), controlState_.rotations_.end(), handle_rotation);

    controlState_.rotations_.clear();
  }

}


