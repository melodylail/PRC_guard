// ========================================================================
// Copyright 2007-2008 Mort Bay Consulting Pty. Ltd.
// ------------------------------------------------------------------------
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//========================================================================

package org.cometd.demo;

import java.io.IOException;
import javax.servlet.GenericServlet;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;

import org.cometd.Bayeux;
import org.cometd.Client;
import org.cometd.Message;
import org.mortbay.cometd.BayeuxService;
import org.mortbay.cometd.ext.AcknowledgedMessagesExtension;
import org.mortbay.cometd.ext.TimesyncExtension;
import org.mortbay.log.Log;

public class CometdDemoServlet extends GenericServlet
{
    public CometdDemoServlet()
    {
    }


    @Override
    public void init() throws ServletException
    {
        super.init();
        Bayeux bayeux=(Bayeux)getServletContext().getAttribute(Bayeux.ATTRIBUTE);
        new EchoRPC(bayeux);
        new Monitor(bayeux);
        new ChatService(bayeux);
        bayeux.addExtension(new TimesyncExtension());
        bayeux.addExtension(new AcknowledgedMessagesExtension());
/*
        StatisticsExtension se = new StatisticsExtension();
        se.setStatsRequestKeys(new String[]{"chat"});
        se.setStatsRequestSentinel("//stats");
        se.setStatsRequestChannel("/chat/demo");
        se.setStatsResultSentinel("//stats-results");
        se.setStatsConfirmSentinel("//stats-confirm");
        se.setProbeChannel("/chat/demo");
        se.setProbeSentinel("//stats-probe");
        se.setProbeReplySentinel("//stats-reply");
        se.setProbeReplyChannel("/chat/demo");
        se.setProbeReplyKeys(new String[]{"chat"});
        bayeux.addExtension(se);
*/
    }

    public static class EchoRPC extends BayeuxService
    {
        public EchoRPC(Bayeux bayeux)
        {
            super(bayeux,"echo");
            subscribe("/service/echo","doEcho");
        }

        public Object doEcho(Client client, Object data)
        {
            Log.info("ECHO from "+client+" "+data);
            return data;
        }
    }

    public static class Monitor extends BayeuxService
    {
        public Monitor(Bayeux bayeux)
        {
            super(bayeux,"monitor");
            subscribe("/meta/subscribe","monitorSubscribe");
            subscribe("/meta/unsubscribe","monitorUnsubscribe");
            subscribe("/meta/*","monitorMeta");
            // subscribe("/**","monitorVerbose");
        }

        public void monitorSubscribe(Client client, Message message)
        {
            Log.info("Subscribe from "+client+" for "+message.get(Bayeux.SUBSCRIPTION_FIELD));
        }

        public void monitorUnsubscribe(Client client, Message message)
        {
            Log.info("Unsubscribe from "+client+" for "+message.get(Bayeux.SUBSCRIPTION_FIELD));
        }

        public void monitorMeta(Client client, Message message)
        {
            if (Log.isDebugEnabled())
                Log.debug(message.toString());
        }

        /*
        public void monitorVerbose(Client client, Message message)
        {
            System.err.println(message);
            try
            {
                Thread.sleep(5000);
            }
            catch(Exception e)
            {
                Log.warn(e);
            }
        }
        */
    }

    @Override
    public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException
    {
        ((HttpServletResponse)res).sendError(503);
    }
}
