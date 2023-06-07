/*******************************************************************************
 (c) 2005-2015 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/

#include <dds/sub/ddssub.hpp>
#include <dds/core/ddscore.hpp>
#include <rti/config/Logger.hpp>  // for logging

#include "waitsets.hpp"
#include "application.hpp"  // for command line parsing and ctrl-c

void run_subscriber_application(
   unsigned int domain_id,
   unsigned int sample_count)
{
   // Create a DomainParticipant with default QoS.
   dds::domain::DomainParticipant participant(domain_id);

   // Create a Topic -- and automatically register the type.
   dds::topic::Topic<waitsets> topic(participant, "Example waitsets");

   // Retrieve the default DataReader QoS, from USER_QOS_PROFILES.xml
   dds::sub::qos::DataReaderQos reader_qos =
       dds::core::QosProvider::Default().datareader_qos();

   // Set policy for reliable reader based on
   // https://community.rti.com/static/documentation/connext-dds/6.1.2/doc/api/connext_dds/api_cpp2/USER_QOS_PROFILES_8xml-example.html
   auto reader_protocol = rti::core::RtpsReliableReaderProtocol()
         .app_ack_period(::dds::core::Duration::from_millisecs(10))
         .max_heartbeat_response_delay(::dds::core::Duration::from_millisecs(0))
         .min_heartbeat_response_delay(::dds::core::Duration::from_millisecs(0))
       ;
    reader_qos << ::dds::core::policy::Reliability::Reliable()
               << ::dds::core::policy::History::KeepLast(10)
               << ::dds::core::policy::Liveliness::Automatic().lease_duration(::dds::core::Duration(2))
               << rti::core::policy::DataReaderProtocol().rtps_reliable_reader(reader_protocol)
       ;

   // Create a Subscriber
   dds::sub::Subscriber subscriber(participant);

   // Create a DataReader.
   dds::sub::DataReader<waitsets> reader(subscriber, topic, reader_qos);

   // Create a Read Condition.
   // Note that the Read Conditions are dependent on both incoming data as
   // well as sample state.
   dds::sub::cond::ReadCondition read_cond(
       reader,
       dds::sub::status::DataState(
           dds::sub::status::SampleState::not_read(),
           dds::sub::status::ViewState::any(),
           dds::sub::status::InstanceState::any()));

   // Create and attach conditions to the WaitSet
   // Finally, we create the WaitSet and attach the Read Conditions
   // to it. We can use the operator '+=' or the method
   // 'attach_condition'.
   dds::core::cond::WaitSet waitset;
   waitset += read_cond;

   // Main loop.
   while (!application::shutdown_requested) {
       // 'wait()' blocks execution of the thread until one or more attached
       // Conditions become true, or until a user-specified timeout expires.
       dds::core::cond::WaitSet::ConditionSeq active_conditions =
           waitset.wait(dds::core::Duration::from_millisecs(1500));

       if (active_conditions.size() == 0) {
           std::cout << "Wait timed out!! No conditions were triggered."
                     << std::endl;
           continue;
       }

       for (std::vector<int>::size_type i = 0; i < active_conditions.size();
            i++) {
           // Now we compare the current condition with the Status Conditions
           // and the Read Conditions previously defined. If they match, we
           // print the condition that was triggered.

           if (active_conditions[i] == read_cond) {
               // Current conditions match our conditions to read data, so we
               // can read data just like we would do in any other example.
               dds::sub::LoanedSamples<waitsets> samples = reader.take();
               for (const auto &sample : samples) {
                   if (!sample.info().valid()) {
                       // Got metadata
                   } else {
                       auto time = std::chrono::system_clock::now();
                       std::time_t current_time = std::chrono::system_clock::to_time_t(time);
                       std::string time_str = std::ctime(&current_time);
                       std::cout  << time_str << sample.data() << std::endl;
                   }
               }
           }
       }

       auto status = reader->datareader_protocol_status();
       std::cout <<
           status.filtered_sample_count()<< " " <<
           status.duplicate_sample_count()<< " " <<
           status.dropped_fragment_count()<< " " <<
           status.first_available_sample_sequence_number()<< " " <<
           status.last_available_sample_sequence_number()<< " " <<
           status.last_committed_sample_sequence_number()<< " " <<
           status.received_sample_count()<< " " <<
           status.sent_ack_count()<< " " <<
           status.sent_nack_count()<< " " <<
           status.rejected_sample_count()<< " " <<
           status.uncommitted_sample_count()<< " " <<
           std::endl;
   }
}

int main(int argc, char *argv[])
{
   using namespace application;

   // Parse arguments and handle control-C
   auto arguments = parse_arguments(argc, argv);
   if (arguments.parse_result == ParseReturn::exit) {
       return EXIT_SUCCESS;
   } else if (arguments.parse_result == ParseReturn::failure) {
       return EXIT_FAILURE;
   }
   setup_signal_handlers();

   // Sets Connext verbosity to help debugging
   rti::config::Logger::instance().verbosity(arguments.verbosity);

   try {
       run_subscriber_application(arguments.domain_id, arguments.sample_count);
   } catch (const std::exception &ex) {
       // This will catch DDS exceptions
       std::cerr << "Exception in run_subscriber_application(): " << ex.what()
                 << std::endl;
       return EXIT_FAILURE;
   }

   // Releases the memory used by the participant factory.  Optional at
   // application exit
   dds::domain::DomainParticipant::finalize_participant_factory();

   return EXIT_SUCCESS;
}