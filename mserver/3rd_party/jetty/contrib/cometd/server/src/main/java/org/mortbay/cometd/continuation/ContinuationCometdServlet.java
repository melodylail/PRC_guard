//========================================================================
//Copyright 2007 Mort Bay Consulting Pty. Ltd.
//------------------------------------------------------------------------
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//========================================================================

package org.mortbay.cometd.continuation;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.cometd.Bayeux;
import org.cometd.Client;
import org.cometd.Message;
import org.mortbay.cometd.AbstractBayeux;
import org.mortbay.cometd.AbstractCometdServlet;
import org.mortbay.cometd.ClientImpl;
import org.mortbay.cometd.JSONTransport;
import org.mortbay.cometd.MessageImpl;
import org.mortbay.cometd.Transport;
import org.mortbay.util.ArrayQueue;
import org.mortbay.util.StringUtil;
import org.mortbay.util.ajax.Continuation;
import org.mortbay.util.ajax.ContinuationSupport;

public class ContinuationCometdServlet extends AbstractCometdServlet
{
    public final static int __DEFAULT_REFS_THRESHOLD=0;
    protected int _refsThreshold=__DEFAULT_REFS_THRESHOLD;
    String _responseBuffer;

    @Override
    public void init() throws ServletException
    {
        String refsThreshold=getInitParameter("refsThreshold");
        if (refsThreshold != null)
            _refsThreshold=Integer.parseInt(refsThreshold);

        if (_refsThreshold>0)
        {
            String server = getServletContext().getServerInfo();
            if (server.startsWith("jetty/6"))
                _responseBuffer="org.mortbay.jetty.ResponseBuffer";
            else if (server.startsWith("jetty/"))
                _responseBuffer="org.eclipse.jetty.server.ResponseBuffer";
            else
                _refsThreshold=0;
        }

        super.init();
    }

    /* ------------------------------------------------------------ */
    @Override
    protected AbstractBayeux newBayeux()
    {
        return new ContinuationBayeux();
    }

    /* ------------------------------------------------------------ */
    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
        if (response.isCommitted())
        {
            // return here as this is a resumed request that has already had a
            // response sent - this is probably a sendError();resume(); used instead
            // of a complete().
            return;
        }

        // Look for an existing client and protect from context restarts
        Object clientObj=request.getAttribute(CLIENT_ATTR);
        Transport transport=null;
        int received=-1;
        boolean metaConnectDeliveryOnly=false;
        boolean pendingResponse=false;
        boolean metaConnect=false;
        final boolean initial;

        // Have we seen this request before
        ContinuationClient client=(clientObj instanceof ClientImpl)?(ContinuationClient)clientObj:null;
        if (client != null)
        {
            initial=false;
            // yes - extract saved properties
            transport=(Transport)request.getAttribute(TRANSPORT_ATTR);
            transport.setResponse(response);
            metaConnectDeliveryOnly=client.isMetaConnectDeliveryOnly() || transport.isMetaConnectDeliveryOnly();
            metaConnect=true;
        }
        else
        {
            initial=true;
            Message[] messages=getMessages(request);
            received=messages.length;

            /* check jsonp parameter */
            String jsonpParam=request.getParameter("jsonp");

            // Handle all received messages
            try
            {
                for (Message message : messages)
                {
                    if (jsonpParam != null)
                        message.put("jsonp",jsonpParam);

                    if (client == null)
                    {
                        String clientId = (String)message.get(AbstractBayeux.CLIENT_FIELD);
                        client=(ContinuationClient)_bayeux.getClient(clientId);

                        // If no client, SHOULD be a handshake, so force a
                        // transport and handle
                        if (client == null)
                        {
                            // Setup a browser ID
                            String browser_id=findBrowserId(request);
                            if (browser_id == null)
                                browser_id=setBrowserId(request,response);

                            if (transport == null)
                            {
                                transport=_bayeux.newTransport(client,message);
                                transport.setResponse(response);
                                metaConnectDeliveryOnly=transport.isMetaConnectDeliveryOnly();
                            }
                            _bayeux.handle(null,transport,message);
                            message=null;
                            continue;
                        }
                    }

                    String browser_id=findBrowserId(request);
                    if (browser_id != null && (client.getBrowserId() == null || !client.getBrowserId().equals(browser_id)))
                        client.setBrowserId(browser_id);

                    // resolve transport
                    if (transport == null)
                    {
                        transport=_bayeux.newTransport(client,message);
                        transport.setResponse(response);
                        metaConnectDeliveryOnly=client.isMetaConnectDeliveryOnly() || transport.isMetaConnectDeliveryOnly();
                    }

                    // Tell client to hold messages as a response is likely to
                    // be sent.
                    if (!metaConnectDeliveryOnly && !pendingResponse)
                    {
                        pendingResponse=true;
                        client.responsePending();
                    }

                    if (Bayeux.META_CONNECT.equals(message.getChannel()))
                        metaConnect=true;

                    _bayeux.handle(client,transport,message);
                }
            }
            finally
            {
                for (Message message : messages)
                    ((MessageImpl)message).decRef();
                if (pendingResponse)
                {
                    client.responded();
                }
            }
        }

        Message metaConnectReply=null;

        // Do we need to wait for messages
        if (transport != null)
        {
            metaConnectReply=transport.getMetaConnectReply();
            if (metaConnectReply != null)
            {
                long timeout=client.getTimeout();
                if (timeout < 0)
                    timeout=_bayeux.getTimeout();

                Continuation continuation=ContinuationSupport.getContinuation(request,client);

                // Get messages or wait
                synchronized(client)
                {
                    if (timeout > 0 && !client.hasNonLazyMessages() && initial && received <= 1)
                    {
                        // save state and suspend
                        request.setAttribute(CLIENT_ATTR,client);
                        request.setAttribute(TRANSPORT_ATTR,transport);
                        client.setContinuation(continuation);
                        continuation.setObject(response);
                        continuation.suspend(timeout);
                    }

                    continuation.reset();
                }

                client.setContinuation(null);
                transport.setMetaConnectReply(null);
            }
            else if (client != null)
            {
                client.access();
            }
        }

        if (client != null)
        {
            if (metaConnectDeliveryOnly && !metaConnect)
            {
                // wake up any long poll
                client.resume();
            }
            else
            {
                List<Message> messages;
                synchronized (client)
                {
                    client.doDeliverListeners();

                    ArrayQueue<Message> clientMessages = (ArrayQueue<Message>)client.getQueue();
                    // Copy the messages to avoid synchronization
                    messages = new ArrayList<Message>(clientMessages);
                    // Empty client's queue
                    clientMessages.clear();
                }

                final int size=messages.size();
                for (int i=0; i < size; i++)
                {
                    final Message message=messages.get(i);
                    final MessageImpl mesgImpl=(message instanceof MessageImpl)?(MessageImpl)message:null;

                    // Can we short cut the message?
                    if (i == 0 && size == 1 && mesgImpl != null && _refsThreshold > 0 && metaConnectReply != null && transport instanceof JSONTransport)
                    {
                        // is there a response already prepared
                        ByteBuffer buffer=mesgImpl.getBuffer();
                        if (buffer != null)
                        {
                            // Send pre-prepared buffer
                            request.setAttribute("org.mortbay.jetty.ResponseBuffer",buffer);
                            if (metaConnectReply instanceof MessageImpl)
                                ((MessageImpl)metaConnectReply).decRef();
                            metaConnectReply=null;
                            transport=null;
                            mesgImpl.decRef();
                            continue;
                        }
                        else if (mesgImpl.getRefs() >= _refsThreshold)
                        {
                            // create multi-use buffer
                            byte[] contentBytes=("[" + mesgImpl.getJSON() + ",{\"" + Bayeux.SUCCESSFUL_FIELD + "\":true,\"" + Bayeux.CHANNEL_FIELD
                                    + "\":\"" + Bayeux.META_CONNECT + "\"}]").getBytes(StringUtil.__UTF8);
                            int contentLength=contentBytes.length;

                            String headerString="HTTP/1.1 200 OK\r\n" + "Content-Type: text/json; charset=utf-8\r\n" + "Content-Length: "
                                    + contentLength + "\r\n" + "\r\n";

                            byte[] headerBytes=headerString.getBytes(StringUtil.__UTF8);

                            buffer=ByteBuffer.allocateDirect(headerBytes.length + contentLength);
                            buffer.put(headerBytes);
                            buffer.put(contentBytes);
                            buffer.flip();

                            mesgImpl.setBuffer(buffer);
                            request.setAttribute("org.mortbay.jetty.ResponseBuffer",buffer);
                            metaConnectReply=null;
                            if (metaConnectReply instanceof MessageImpl)
                                ((MessageImpl)metaConnectReply).decRef();
                            transport=null;
                            mesgImpl.decRef();
                            continue;
                        }
                    }

                    if (message != null)
                        transport.send(message);
                    if (mesgImpl != null)
                        mesgImpl.decRef();
                }

                if (metaConnectReply != null)
                {
                    metaConnectReply=_bayeux.extendSendMeta(client,metaConnectReply);
                    transport.send(metaConnectReply);
                    if (metaConnectReply instanceof MessageImpl)
                        ((MessageImpl)metaConnectReply).decRef();
                }
            }
        }

        if (transport != null)
            transport.complete();
    }

    public void destroy()
    {
        ContinuationBayeux bayeux = (ContinuationBayeux)_bayeux;
        if (bayeux != null)
        {
            for (Client c : bayeux.getClients())
            {
                if (c instanceof ContinuationClient)
                {
                    ContinuationClient client = (ContinuationClient)c;
                    client.setContinuation(null);
                }
            }
            bayeux.destroy();
        }
    }
}
