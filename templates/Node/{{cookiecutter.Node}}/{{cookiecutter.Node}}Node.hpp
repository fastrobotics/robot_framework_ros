/**
 * @file {{cookiecutter.Node}}Node.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-07-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <robot_framework_ros/BaseNode.hpp>
#include <{{cookiecutter.Node}}Process/{{cookiecutter.Node}}Process.hpp>
namespace fast::rf_ros::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem {
    /**
     * @brief {{cookiecutter.Node}} Node
     *
     */
    class {{cookiecutter.Node}}Node : public BaseNode {
       public:
        {{cookiecutter.Node}}Node();
        ~{{cookiecutter.Node}}Node();

        /**
         * @brief Initialize the Node
         *
         * @return true
         * @return false
         */
        bool init();

        /**
         * @brief Start the Node
         *
         * @return true
         * @return false
         */
        bool start();

        /**
         * @brief 100Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_100hz();

        /**
         * @brief 10Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_10hz();

        /**
         * @brief 1Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_1hz();

        /**
         * @brief 0.1Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_01hz();

        /**
         * @brief 0.01Hz Loop Function
         *
         * @return true
         * @return false
         */
        bool run_001hz();

        /**
         * @brief User Function 1
         *
         * @return true
         * @return false
         */
        bool run_loop1();

        /**
         * @brief User Function 2
         *
         * @return true
         * @return false
         */
        bool run_loop2();

        /**
         * @brief User Function 3
         *
         * @return true
         * @return false
         */
        bool run_loop3();

        /**
         * @brief User Thread Loop
         *
         */
        void thread_loop();


       private:
        fast::rf::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem::{{cookiecutter.Node}}Process
            process;  //!< Execution Process
    };
}  // namespace fast::rf_ros::{{cookiecutter.System}}System::{{cookiecutter.Subsystem}}Subsystem
