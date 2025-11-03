/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:03:19 by gabastos          #+#    #+#             */
/*   Updated: 2025/11/03 09:03:20 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <memory>
#include "Channel.hpp"
#include "EventLoop.hpp"
#include "EventLoopThreadPool.hpp"

class Server {
	private:
		EventLoop *loop_;
		int threadNum_;
		std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
		bool started_;
		std::shared_ptr<Channel> acceptChannel_;
		int port_;
		int listenFd_;
		static const int MAXFDS = 100000;
 	public:
		Server(EventLoop *loop, int threadNum, int port);
		~Server() {}
		EventLoop *getLoop() const { return loop_; }
		void start();
		void handNewConn();
		void handThisConn() { loop_->updatePoller(acceptChannel_); }

};