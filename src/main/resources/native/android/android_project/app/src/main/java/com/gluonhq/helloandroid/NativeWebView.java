/*
 * Copyright (c) 2020, Gluon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLUON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.gluonhq.helloandroid;

import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.webkit.WebView;

public class NativeWebView {

    private static MainActivity instance;
    private WebView webView;
    private boolean inlayout = false;

    public NativeWebView() {
System.err.println("NATIVEWEBVIEW xonstructor starts");
        instance = MainActivity.getInstance();
        instance.runOnUiThread(new Runnable () {
            public void run() {
                NativeWebView.this.webView = new WebView(instance);
System.err.println("NATIVEWEBVIEW wv = "+NativeWebView.this.webView);
System.err.println("finally, NATIVEWEBVIEW wv = "+NativeWebView.this.webView);
            }
        });
        reLayout();
System.err.println("NATIVEWEBVIEW xonstructor returns: "+this);
    }

    public void loadUrl(final String url) {
System.err.println("in dalvik, loadUrl called wwith url = "+url+" and webView = "+this.webView);
        instance.runOnUiThread(new Runnable () {
            public void run() {
        NativeWebView.this.webView.loadUrl(url);
            }
        });
    }

    private void reLayout() {
System.err.println("relayout...");
        if (!inlayout) {
            instance.runOnUiThread(new Runnable () {
                public void run() {
                    FrameLayout.LayoutParams layout = new FrameLayout.LayoutParams(
                            ViewGroup.LayoutParams.WRAP_CONTENT,
                            ViewGroup.LayoutParams.WRAP_CONTENT,
                            Gravity.NO_GRAVITY);
                    MainActivity.getViewGroup().addView(webView, layout);
                    inlayout = true;
                }
            });
        }
        instance.runOnUiThread(new Runnable () {
            public void run() {
                FrameLayout.LayoutParams layout =
                        (FrameLayout.LayoutParams) webView.getLayoutParams();
                    // layout.leftMargin = x;
                    // layout.topMargin = y;
                    // layout.width = width;
                    // layout.height = height;
                    MainActivity.getViewGroup().updateViewLayout(webView, layout);
                }
            });

    }
}
